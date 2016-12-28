//============================================================================
// Name        : watchfolder.cpp
// Author      : André Cajús
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/inotify.h>

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

int main( int argc, char **argv )
{
    int length, i = 0;
    int fd;
    int wd[2];
    char buffer[BUF_LEN];

    fd = inotify_init();

    if ( fd < 0 ) {
        perror( "inotify_init" );
    }

    wd[0] = inotify_add_watch( fd, "/home/andrecajus/Desktop/watchFolder", IN_CREATE);
    wd[1] = inotify_add_watch (fd, "/home/andrecajus/Desktop/watchFolder", IN_CREATE);

    while (1){
        struct inotify_event *event;

        length = read( fd, buffer, BUF_LEN );

        if ( length < 0 ) {
            perror( "read" );
        }

        event = ( struct inotify_event * ) &buffer[ i ];

        if ( event->len ) {
            if (event->wd == wd[0]) printf("%s\n", "WHATTTTTTT AQUI: ");
            else printf("%s\n", "RLY?: ");
            if ( event->mask & IN_CREATE ) {
                if ( event->mask & IN_ISDIR ) {
                    printf( "The directory %s was created.\n", event->name );
                }
                else {
                    printf( "The file %s was created.\n", event->name );
                }
            }
        }
    }
    ( void ) inotify_rm_watch( fd, wd[0] );
    ( void ) inotify_rm_watch( fd, wd[1]);
    ( void ) close( fd );

    exit( 0 );
}
