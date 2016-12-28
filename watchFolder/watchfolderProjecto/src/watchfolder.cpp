//============================================================================
// Name        : watchfolder.cpp
// Author      : André Cajús
// Version     :
// Copyright   : 
// Description : C++, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <string>

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )
bool has_suffix(const std::string &str, const std::string &suffix);

int main(int argc, char **argv) {
	int length, i = 0;
	int fd;
	int wd;
	char buffer[BUF_LEN];

	std::string fn = ".m4v";

	fd = inotify_init();

	if (fd < 0) {
		perror("inotify_init");
	}

	//exemplo de vários eventos, sendo o IN_MOVE o principal
	wd = inotify_add_watch(fd, "/home/andrecajus/Desktop/watchFolder",
	IN_MODIFY | IN_CREATE | IN_DELETE | IN_MOVE);

	//está sempre a correr, uma vez que é o intuito do watchfolder
	while (1) {

		//uso de inotify events (documentação online)
		struct inotify_event *event;

		length = read(fd, buffer, BUF_LEN);

		if (length < 0) {
			perror("read");
		}

		event = (struct inotify_event *) &buffer[i];

		if (event->len) {
			if (event->mask & IN_CREATE) {
				if (event->mask & IN_ISDIR) {
					printf("O directorio %s foi criado!\n", event->name);
				} else {
					printf("O ficheiro %s foi criado!\n", event->name);
				}
			} else if (event->mask & IN_DELETE) {
				if (event->mask & IN_ISDIR) {
					printf("O directorio %s foi apagado!\n", event->name);
				} else {
					printf("O ficheiro %s foi apagado!\n", event->name);
				}
			} else if (event->mask & IN_MODIFY) {
				if (event->mask & IN_ISDIR) {
					printf("O directorio %s foi modificado!\n", event->name);
				} else {
					printf("O ficheiro %s foi modificado!\n", event->name);
				}
			}
			//IN_MOVED_TO será o if essencial porque é o que vai acionar o algoritmo de imagem sempre que introduzido um ficheiros .m4v
			else if (event->mask & IN_MOVED_TO) {
				if (event->mask & IN_ISDIR) {
					printf("Directorio %s movido!\n", event->name);
				} else {
					//passa o nome do file em char[] para string
					std::string filename(event->name);

					//chama função que verifica se o file(filename) acaba em .m4v(fn)
					if (has_suffix(filename, fn) == true) {
						//AQUI SERÁ CHAMADO O ALGORITMO DE IMAGEM
						printf("\nAQUI SERÁ CHAMADO O ALGORITMO DE IMAGEM\n");
					}

					printf("Ficheiro %s movido!\n", event->name);
				}
			}
		}

	}

	(void) inotify_rm_watch(fd, wd);
	(void) close(fd);

	exit(0);
}

//para verificar os ficheiros que terminam em .m4v
bool has_suffix(const std::string &str, const std::string &suffix) {
	return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

