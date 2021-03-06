#include "headers/url.h"

char * rewrite_url(char * url)
{
	char *rewritten;
	int i;
	
	/* Accès interdit si l'url contient le token voulu */
	if (strstr(url, "/../") != NULL)
		return NULL;
	
	/* Retourne à la page d'index si l'url est la racine */
	if (strcmp(url, "/") == 0)
		return "/index.html";
	
	i = 0;
	while (url[i] != '\0' && url[i] != '?')
	{
		i++;
	}
	
	/* url[i] == '\0' ou url[i] == '?' */
	rewritten = (char *)malloc(i+1);
	
	if (rewritten == NULL)
	{
		perror("malloc");
		return NULL;
	}
	
	strncpy(rewritten, url, i);
	rewritten[i] = '\0';
	
	return rewritten;
}

int check_and_open(const char * url, const char * document_root)
{
	int fd;
	char *path;
	struct stat s;
	int status;
	
	path = (char*)malloc(strlen(url)+strlen(document_root)+1);
	
	strcpy(path, document_root);
	strcat(path, url);
	
	status = stat(path, &s);
	
	if (status == -1)
		return -1;
	
	if (!S_ISREG(s.st_mode)) {
		errno = EACCES;
		return -1;
	}
	
	fd = open(path, O_RDONLY);
	
	if (fd < 0)
	{
		perror("open");
		return -1;
	}
	
	return fd;
}

int get_file_size(int fd)
{
	struct stat buf;
	
	if (fstat(fd, &buf) == -1)
	{
		perror("fstat");
		return -1;
	}
	
	return buf.st_size;
}

int copy(int in, int out)
{
	int towrite;
	char buf[8000];

	towrite = read(in,buf,sizeof(buf));
	while (towrite > 0)
	{
		write(out,buf,towrite);
		towrite = read(in,buf,sizeof(buf));
	}	
	return out;
}
