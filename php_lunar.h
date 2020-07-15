/* lunar extension for PHP (c) 2020 2233466866 */

#ifndef PHP_LUNAR_H
# define PHP_LUNAR_H

extern zend_module_entry lunar_module_entry;
# define phpext_lunar_ptr &lunar_module_entry

# define PHP_LUNAR_VERSION "0.1.0"

# if defined(ZTS) && defined(COMPILE_DL_LUNAR)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif	/* PHP_LUNAR_H */

