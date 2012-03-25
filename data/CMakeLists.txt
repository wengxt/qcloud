
foreach(size 48)
    install(DIRECTORY icons/${size}x${size} DESTINATION share/icons/hicolor
             PATTERN .* EXCLUDE
             PATTERN *~ EXCLUDE)
endforeach(size 48)

install(DIRECTORY icons/scalable DESTINATION share/icons/hicolor
         PATTERN .* EXCLUDE
         PATTERN *~ EXCLUDE)
