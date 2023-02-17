#include "shell.h"

/**
 * bfree - frees a pointer and NULL the address
 * @ptr: addres of the pointer to be free
 *
 * Return: 1 if freed else 0
 */
int bfree(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
		return (1);
	}
	return (0);
}
