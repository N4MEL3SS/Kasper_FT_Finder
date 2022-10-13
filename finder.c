#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define END "\033[0m"

// Указатель для хранения имени программы, берётся из argv[0]
char *prog_name;
/*
 Флаг для отображения файлов и папок в консоли
 При наличии 3 аргумента вывод будет древовидным, иначе список
*/
int display_method;

typedef struct s_dir_data
{
	DIR *dir_subpath;
	struct dirent *dir_ent;
	char sub_path[PATH_MAX];
}	dir_data;

// Функция для вывода древовидного каталога, для использования необходимо указать 3 аргумент
void tree_print(int lvl)
{
	if (display_method)
	{
		while (lvl > 1)
		{
			printf("  ");
			lvl--;
		}
		if (lvl > 0)
			printf("  |--");
	}
}

// Функция для создания пути к вложенным папкам
int copy_subpath(dir_data *data, char *path)
{
	if ((strlen(path) + strlen(data->dir_ent->d_name) + 1) < PATH_MAX)
	{
		strcpy(data->sub_path, path);
		strcat(data->sub_path, "/");
		strcat(data->sub_path, data->dir_ent->d_name);
	}
	else
		return 1;

	return 0;
}

void finder(DIR *dir_path, char *path, dir_data data, int tree_lvl)
{
	while ((data.dir_ent = readdir(dir_path)) != NULL)
	{
		if (strcmp(data.dir_ent->d_name, ".") == 0 || strcmp(data.dir_ent->d_name, "..") == 0)
			continue;

		// Создаем путь к вложенной папке
		if (copy_subpath(&data, path))
		{
			printf("%sThe path name exceeds the allowed length:%s %d\n", RED, END, PATH_MAX);
			printf("%s/%s\n", path, data.dir_ent->d_name);
			return;
		}

		// Пытаемся открыть вложенную папку, если это удалось вызываем рекурсивно функцию finder
		if ((data.dir_subpath = opendir(data.sub_path)) != NULL)
		{
			tree_print(tree_lvl);
			printf("%s/%s%s%s\n", path, BLUE, data.dir_ent->d_name, END);
			finder(data.dir_subpath, data.sub_path, data, tree_lvl + 1);
		}
		// Если папку открыть не удалось, проверяем не является ли это файлом
		else if (errno == ENOTDIR)
		{
			tree_print(tree_lvl);
			printf("%s/%s%s%s\n", path, GREEN, data.dir_ent->d_name, END);
		}
		// В ином случае выводим ошибку
		else
		{
			printf("%s/%s%s%s\n", path, BLUE, data.dir_ent->d_name, END);
			printf("%s%s: '%s': %s%s\n", RED, prog_name, data.sub_path, \
			strerror(errno), END);
		}
	}
	closedir(dir_path);
}

int main(int argc, char *argv[])
{
	dir_data data = {NULL, NULL, "\0"};
	DIR *dir_path;
	int tree_lvl = 0;

	// Копируем название программы в буфер для использования при выводе ошибок
	prog_name = strrchr(argv[0], '/') + 1;
	display_method = 0;

	if (argc == 3)
		display_method = 1;

	if (argc == 2 || display_method)
	{
		if ((dir_path = opendir(argv[1])) != NULL)
		{
			printf("%s\n", argv[1]);
			finder(dir_path, argv[1], data, tree_lvl);
		}
		else
			printf("%s%s: '%s': %s%s\n", RED, prog_name, argv[1], strerror(errno), END);
	}
	else
	{
		printf("%s-----Error-----%s\n", RED, END);
		printf("%sUsage: ./%s <path_to_folder> %s<optional: list_or_tree (0, 1)>%s\n", GREEN,
			prog_name, BLUE, END);
	}

	return 0;
}
