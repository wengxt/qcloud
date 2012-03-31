#include <cstdio>

#include <archive_entry.h>

#include <QDir>
#include <QDebug>
#include <QDateTime>

#include "packager.h"

#define BUF_SIZE 8192
#define BLOCK_SIZE 65536

namespace QCloud
{

static void scanDir(QStringList& fileList, const QString& path)
{
    QDir dir(path);

    if (!dir.exists())
        return;

    QDir::Filters filterflag = QDir::Files | QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot;

    QStringList list = dir.entryList(filterflag);
    Q_FOREACH(const QString & item, list) {
        QString absItem = dir.absoluteFilePath(item);
        QFileInfo info(absItem);
        if (info.isDir())
            scanDir(fileList, absItem);
        else
            fileList << absItem;
    }
}

Packager::Packager()
{

}

Packager::~Packager()
{

}

bool Packager::compress (const QString& fileName, const QString& baseDir, const QStringList& fileList, bool ignoreError)
{
    QDir dir(baseDir);
    if (!dir.exists())
        return false;

    QStringList expandFileList;

    Q_FOREACH (const QString& f, fileList)
    {
        QFileInfo file(f);
        if (!file.exists()) {
            qDebug() << f << " not exists";
            if (ignoreError)
                continue;
            else
                return false;
        }

        QString relPath = dir.relativeFilePath(f);
        if (relPath.startsWith("..")) {
            qDebug() << f << " is not child of " << dir.path();
            if (ignoreError)
                continue;
            else
                return false;
        }

        if (file.isDir()) {
            scanDir(expandFileList, f);
        }
        else {
            expandFileList << f;
        }
    }

    struct archive* a = archive_write_new();
    archive_write_add_filter_gzip (a);
    archive_write_set_format_pax (a);
    if (archive_write_open_filename (a, (fileName.toLocal8Bit()).constData()) != ARCHIVE_OK) {
        qDebug() << "archive_write_open_filename() error";
        archive_write_free (a);
        return false;
    }

    struct archive_entry* entry;
    char buff[BUF_SIZE];
    Q_FOREACH (const QString & item , expandFileList) {
        QString relPath = dir.relativeFilePath(item);

        struct stat st;
        stat(item.toLocal8Bit().constData(), &st);

        entry = archive_entry_new();
        archive_entry_set_pathname (entry, (relPath.toLocal8Bit()).constData());
        archive_entry_set_size(entry, st.st_size);
        archive_entry_set_filetype(entry, AE_IFREG);
        archive_entry_set_perm(entry, st.st_mode);
        archive_entry_set_mtime(entry, st.st_mtime, 0);
        archive_write_header (a, entry);
        QFile file(item);
        do {
            if (!file.open(QIODevice::ReadOnly)) {
                qDebug() << "open " << item << " failed";
                break;
            }
            qint64 size;
            while ((size = file.read(buff, BUF_SIZE)) > 0) {
                archive_write_data(a, buff, size);
            }
            file.close();
        } while(0);
        archive_write_finish_entry(a);
        archive_entry_free (entry);
    }
    archive_write_close(a);
    archive_write_free(a);
    return true;
}

bool Packager::extract (const QString& fileName, const QString& outputPath)
{
    QFileInfo finfo(outputPath);

    if (!finfo.exists() || !finfo.isDir()) {
        return false;
    }

    QDir dir(outputPath);

    struct archive* a = archive_read_new();
    archive_read_support_format_all(a);
    archive_read_support_compression_all(a);
    if (archive_read_open_filename (a, (fileName.toLocal8Bit()).constData(), BLOCK_SIZE) != ARCHIVE_OK) {
        qDebug() << "open " << fileName << " failed.";
        archive_read_free (a);
        return false;
    }

    /* Select which attributes we want to restore. */
    int flags = ARCHIVE_EXTRACT_TIME;
    flags |= ARCHIVE_EXTRACT_PERM;
    flags |= ARCHIVE_EXTRACT_ACL;
    flags |= ARCHIVE_EXTRACT_FFLAGS;

    struct archive* ext = archive_write_disk_new();
    archive_write_disk_set_options(ext, flags);
    archive_write_disk_set_standard_lookup(ext);

    bool result = true;
    struct archive_entry* entry = 0;
    for (;;) {
        int r = archive_read_next_header(a, &entry);
        if (r == ARCHIVE_EOF)
            break;
        if (r != ARCHIVE_OK)
            qDebug() << archive_error_string(a);
        if (r < ARCHIVE_WARN) {
            result = false;
            break;
        }
        qDebug() << archive_entry_pathname(entry);

        QString absPath = dir.absoluteFilePath(QString::fromLocal8Bit(archive_entry_pathname(entry)));

        archive_entry_set_pathname(entry, absPath.toLocal8Bit().constData());

        r = archive_read_extract2 (a, entry, ext);

        if (r != ARCHIVE_OK)
            qDebug() << archive_error_string(a);
        if (r < ARCHIVE_WARN) {
            result = false;
            break;
        }
    }
    archive_read_close (a);
    archive_read_free (a);
    archive_write_close(ext);
    archive_write_free(ext);

    return result;
}

}
