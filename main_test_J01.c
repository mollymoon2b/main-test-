/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test_J01.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ade-bonn <ade-bonn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/18 20:37:44 by ade-bonn          #+#    #+#             */
/*   Updated: 2015/02/18 22:48:27 by ade-bonn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>
#include "get_next_line.h"
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
/*
void		check_ex01(void)
{
	int		fd;
	char	*str;
	char	*tmp;

	if (!(fd = open("ex01/42", O_RDONLY)))
		printf("\e[31mFile ex01/42 does not exist\n\e[0m");
	if (get_next_line(fd, &str) <= 0)
		printf("\e[31mFile ex01/42 seems empty\n\e[0m");
	{
		if (get_next_line(fd, &tmp) > 0)
			printf("\e[31mFile ex01/42 seems too big\n\e[0m");
		else
		{
			if (strcmp(str, "Coucou 42"))
				printf("\e[31mThe content of ex01/42 seems wrong\n\e[0m");
			else
				printf("\e[32mEx01/42 : OK\n\e[0m");
		}
	}
	}*/

char *ft_dojoin(char const *s1, char const *s2)
{
	register char*str;
	register char*ptr;

	if (s1 && !s2)
		return (ft_strdup(s1));
	else if (!s1 && s2)
		return (ft_strdup(s2));
	else if (!s1)
		return (NULL);
	if (!(str = (char *)malloc(sizeof(char) * \
							   (ft_strlen(s1) + ft_strlen(s2) + 1))))
		return (NULL);
	ptr = str;
	while (*s1)
		*ptr++ = *s1++;
	while (*s2)
		*ptr++ = *s2++;
	*ptr = '\0';
	return (str);
}

char		*ft_getfile(char *path)
{
    int fd;
	char *buff;
	char *line;
	char *tmp;

	if (!(fd = open(path, O_RDONLY)))
    {
        printf("File %s can't be opened\n", path);
        return (NULL);
    }
	buff = NULL;
    while ((get_next_line(fd, &line) > 0) || (line && *line))
    {
		printf("Looping : %s\n", line);
        tmp = ft_dojoin(buff, line);
        buff = tmp;
		line = NULL;
    }
	return (buff);
}

void        check_ex01(void)
{
    char    *str;

	str = ft_getfile("ex01/42");
	if (!ft_strcmp(str, "Coucou 42"))
		printf("\e[32mEx01/42 : OK\n\e[0m");
	else
		printf("\e[31mThe content of ex01/42 seems wrong\n\e[0m");
}

static char *ft_get_word(char **str, char quote)
{
	char *ptr;
	char *word;

	if (!(word = (char *)ft_memalloc(sizeof(char) * ft_strlen(*str))))
		return (NULL);
	ptr = word;
	while (*(*str))
	{
		if (!quote && *(*str) == '\\')
			*ptr++ = *++(*str);
		else
		{
			if (!quote && (*(*str) == '\'' || *(*str) == '"'))
				quote = *(*str);
			else if (quote && *(*str) == quote)
				quote = '\0';
			else if (!quote && (*(*str) == ' ' || *(*str) == '\t'))
				break ;
			else
				*ptr++ = *(*str);
		}
		(*str)++;
	}
	*ptr = '\0';
	return (word);
}

static size_t ft_parse_len(char *str)
{
	char *tmp;
	size_t v;

	v = 0;
	while (*str)
	{
		tmp = ft_get_word(&str, '\0');
		free(tmp);
		while (*str == ' ' || *str == '\t')
			str++;
		v++;
	}
	return (v);
}

char **ft_parse_args(char *input)
{
	char **argv;
	char **ptr;
	int len;

	len = ft_parse_len(input);
	if (!(argv = (char **)malloc(sizeof(char *) * len + 1)))
		return (NULL);
	ptr = argv;
	while (len-- && *input)
	{
		*ptr = ft_get_word(&input, '\0');
		if (**ptr)
			ptr++;
		else
			free(*ptr);
		while (*input == ' ' || *input == '\t')
			input++;
	}
	*ptr = NULL;
	return (argv);
}


int ft_exec(char *code, char **envp)
{
	char	**argc;
	char	*path;
	pid_t	cpid;

	argc = ft_parse_args(code);
	path = ft_dojoin("/bin/", argc[0]);
	if (access(path, X_OK))
	{
		printf("%s can't be opened, unknown function\n", path);
		return (0);
	}
	cpid = fork();
	if (cpid != -1)
	{
		if (cpid == 0)
			execve(path, argc + 1, envp);
		else
			waitpid(cpid, 0, 0);
		free(path);
		return (1);
	}
	return (0);
}

void		check_exo(char *userpath, char *realcode, char *exo, char **envp)
{
	char *usercode;
	char *user;
	char *real;
	FILE *fp;
	
	usercode = ft_getfile(userpath);
	if (!usercode || !*usercode)
	{
		printf("Code = %s\n", usercode);
		printf("File %s seem not to exist\n", userpath);
		return ;
	}
	fp = freopen("tmp1", "w+", stdout);
	ft_exec(usercode, envp);
	free(usercode);
	fclose(fp);
	fp = freopen("tmp2", "w+", stdout);
    ft_exec(realcode, envp);
    fclose(fp);
	freopen ("/dev/tty", "a", stdout);
	user = ft_getfile("tmp1");
	real = ft_getfile("tmp2");
//	remove("tmp1");
//	remove("tmp2");
	if (!ft_strcmp(user, real))
		printf("Exercice %s valide !\n", exo);
	else
		printf("Exercice %s invalide ! : (reel) : %s, contre (vous) %s\n", exo, real, user);
	free(user);
	free(real);
}

int		main(int ac, char **av, char **envp)
{
	check_ex01();
	check_exo("ex02/exo2.sh", "find . -size +100kb | egrep '\\.docx$'", "ex02/exo2.sh", envp);
}
