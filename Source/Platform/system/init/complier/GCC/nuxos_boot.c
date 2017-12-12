/*!
********************************************************************************
* @file     : nuxos_boot.c
*
* @brief    :
*
* @version  : V0.0.1
*
* @copyright: All Rights Reserved.
********************************************************************************
* @changelog: L.H.T 2017-10-01 first version
*******************************************************************************/
#include <menux/nuxos.h>

extern int __real_main(void);
extern void nuxos_startup(void);
extern int main(int argc, char* argv[]);
extern void __libc_init_array (void);

void nuxos_premain(void *argument)
{
	__libc_init_array();

	main(0, DEF_NULL);
}

/*!
********************************************************************************
* @brief :
*
* @param :
*
* @retval:
*******************************************************************************/
int __wrap_main(void)
{
    return __real_main();
}

/*!
********************************************************************************
* @brief :
*
* @param :
*
* @retval:
*******************************************************************************/
void software_init_hook(void)
{
	nuxos_startup();
}
