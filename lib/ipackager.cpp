#include "ipackager.h"
#include <archive_entry.h>
#include <cstdio>
#define BLOCK_SIZE 65536

namespace QCloud{
    
IPackager::IPackager()
{
    
}

IPackager::~IPackager()
{
    
}

bool IPackager::CompressFile(const QString& FileName,const QStringList& FileList)
{
    archive* m_archive = archive_read_new();
    archive_write_set_compression_gzip(m_archive);
    if (archive_write_open_filename(m_archive,(FileName.toLocal8Bit()).constData())
        !=ARCHIVE_OK){
        archive_read_close(m_archive);
        return false;
    }
    
    archive_entry* m_archive_entry;
    foreach (QString st,FileList){
        m_archive_entry = archive_entry_new();
        archive_entry_set_pathname(m_archive_entry,(st.toLocal8Bit()).constData());
        if (archive_write_header(m_archive,m_archive_entry)!=ARCHIVE_OK){
            archive_write_close(m_archive);
            archive_entry_free(m_archive_entry);
            return false;
        }
        archive_entry_free(m_archive_entry);
    }
    return true;
}

bool IPackager::ExtractFile(const QString& FileName,const QString& OutputPath)
{
    archive* m_archive = archive_read_new();
    if (archive_read_open_filename(m_archive,(FileName.toLocal8Bit()).constData(),BLOCK_SIZE)
        !=ARCHIVE_OK){
        archive_read_close(m_archive);
        return false;
    }
    
    archive_entry* m_archive_entry = archive_entry_new();
    archive_entry_set_pathname(m_archive_entry,(OutputPath.toLocal8Bit()).constData());
    
    if (archive_read_extract(m_archive,m_archive_entry,ARCHIVE_EXTRACT_OWNER 
        | ARCHIVE_EXTRACT_PERM | ARCHIVE_EXTRACT_SECURE_NODOTDOT)
        !=ARCHIVE_OK){
        archive_entry_free(m_archive_entry);
        archive_read_close(m_archive);
        return false;
    }
    archive_entry_free(m_archive_entry);
    
    if (archive_read_finish(m_archive)!=ARCHIVE_OK)
        return false;
    return true;
}
    
}