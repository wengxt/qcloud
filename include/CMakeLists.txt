install(DIRECTORY QtCloud DESTINATION include
         PATTERN .* EXCLUDE
         PATTERN *~ EXCLUDE)

set(qcloud_HEADER
    ibackend.h
    oauthbackend.h
    oauthwidget.h
    reply.h
    factory.h
    isecurestore.h
    isecurestorewidget.h
    server.h
    client.h
    authdialog.h
    authwidget.h
    appmanager.h
    )

install(FILES ${qcloud_HEADER} DESTINATION include)