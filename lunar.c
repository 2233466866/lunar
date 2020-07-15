#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_lunar.h"

#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
    ZEND_PARSE_PARAMETERS_START(0, 0) \
    ZEND_PARSE_PARAMETERS_END()
#endif

/******************************↓******************************/
static char *lunar_tiangan_names[] = {"甲","乙","丙","丁","戊","己","庚","辛","壬","癸"};
static char *lunar_dizhi_names[] = {"子","丑","寅","卯","辰","巳","午","未","申","酉","戌","亥"};
static char *lunar_month_names[] = {" ","正","二","三","四","五","六","七","八","九","十","冬","腊"};
static char *lunar_day_names[] = {" ","初一","初二","初三","初四","初五","初六","初七","初八","初九","初十","十一","十二","十三","十四","十五","十六","十七","十八","十九","二十","廿一","廿二","廿三","廿四","廿五","廿六","廿七","廿八","廿九","三十"};
static int hex_lunar[] = {0x04bd8,0x04ae0,0x0a570,0x054d5,0x0d260,0x0d950,0x16554,0x056a0,0x09ad0,0x055d2,0x04ae0,0x0a5b6,0x0a4d0,0x0d250,0x1d255,0x0b540,0x0d6a0,0x0ada2,0x095b0,0x14977,0x04970,0x0a4b0,0x0b4b5,0x06a50,0x06d40,0x1ab54,0x02b60,0x09570,0x052f2,0x04970,0x06566,0x0d4a0,0x0ea50,0x06e95,0x05ad0,0x02b60,0x186e3,0x092e0,0x1c8d7,0x0c950,0x0d4a0,0x1d8a6,0x0b550,0x056a0,0x1a5b4,0x025d0,0x092d0,0x0d2b2,0x0a950,0x0b557,0x06ca0,0x0b550,0x15355,0x04da0,0x0a5d0,0x14573,0x052d0,0x0a9a8,0x0e950,0x06aa0,0x0aea6,0x0ab50,0x04b60,0x0aae4,0x0a570,0x05260,0x0f263,0x0d950,0x05b57,0x056a0,0x096d0,0x04dd5,0x04ad0,0x0a4d0,0x0d4d4,0x0d250,0x0d558,0x0b540,0x0b5a0,0x195a6,0x095b0,0x049b0,0x0a974,0x0a4b0,0x0b27a,0x06a50,0x06d40,0x0af46,0x0ab60,0x09570,0x04af5,0x04970,0x064b0,0x074a3,0x0ea50,0x06b58,0x055c0,0x0ab60,0x096d5,0x092e0,0x0c960,0x0d954,0x0d4a0,0x0da50,0x07552,0x056a0,0x0abb7,0x025d0,0x092d0,0x0cab5,0x0a950,0x0b4a0,0x0baa4,0x0ad50,0x055d9,0x04ba0,0x0a5b0,0x15176,0x052b0,0x0a930,0x07954,0x06aa0,0x0ad50,0x05b52,0x04b60,0x0a6e6,0x0a4e0,0x0d260,0x0ea65,0x0d530,0x05aa0,0x076a3,0x096d0,0x04bd7,0x04ad0,0x0a4d0,0x1d0b6,0x0d250,0x0d520,0x0dd45,0x0b5a0,0x056d0,0x055b2,0x049b0,0x0a577,0x0a4b0,0x0aa50,0x1b255,0x06d20,0x0ada0,0x14b63};

int leap_month(int year)
{
    return hex_lunar[year-1900] & 0xf;
}

int leap_days(int year)
{
    if (leap_month(year)) {
        return (hex_lunar[year-1900] & 0x10000)? 30 : 29;
    }
    return 0;
}

int total_days_num_of_year(int year)
{
    int sum = 348;
    int i;
    for (i = 0x8000; i > 0x8; i >>= 1) {
        sum += (hex_lunar[year-1900] & i) ? 1 : 0;
    }
    return sum + leap_days(year);
}

int month_days(int year, int month)
{
    return hex_lunar[year-1900] & (0x10000>>month) ? 30:29;
}
/******************************↑******************************/

PHP_FUNCTION(date_to_lunar)
{
/******************************↓******************************/
    int argc = ZEND_NUM_ARGS();

    long year;
    long month;
    long day;

    if (zend_parse_parameters(argc TSRMLS_CC, "lll", &year, &month, &day) == FAILURE) return;

    int offset = 0;
    int temp = 0;
    int leap = 0;
    int is_leap = 0;
    int i = 0;
    struct tm info;
    long timestamp;

    int lunar_day;
    int lunar_month;
    int lunar_year;

    int day_cyl;
    int mon_cyl;
    int year_cyl;

    info.tm_year = year - 1900;
    info.tm_mon = month - 1;
    info.tm_mday = day;

    info.tm_hour = 0;
    info.tm_min = 0;
    info.tm_sec = 0;

    info.tm_isdst = -1;
    timestamp = mktime(&info);

    if (year < 1970) {
        temp = 0;
        for (i = 1970; i > year; i--) {
            temp = total_days_num_of_year(i);
            offset -= temp;
        }
    }

    offset = round((double)timestamp / 86400 + 25537);
    day_cyl = offset + 40;
    mon_cyl = 14;

    for (i = 1900; i < year && offset > 0; i++) {
        temp = total_days_num_of_year(i);
        offset -= temp;
        mon_cyl += 12;
    }

    if (offset < 0) {
        offset += temp;
        i--;
        mon_cyl -= 12;
    }

    lunar_year = i;
    year_cyl = i - 1864;

    leap = leap_month(i);
    is_leap = 0;

    for (i = 1; i < 13 && offset > 0; i++) {
        // 闰月
        if (leap > 0 && i == (leap+1) && is_leap == 0) {
            i--;
            is_leap = 1;
            temp = leap_days(lunar_year);
        } else {
            temp = month_days(lunar_year, i);
        }
        // 解除闰月
        if (is_leap == 1 && i == (leap +1)) {
            is_leap = 0;
        }
        offset -= temp;
        if (is_leap == 0) {
            mon_cyl ++;
        }
    }
    if (offset == 0 && leap > 0 && i==(leap+1)) {
        if (is_leap == 1) {
            is_leap = 0;
        } else {
            is_leap = 1;
            i--;
            mon_cyl --;
        }
    }
    if (offset < 0) {
        offset += temp;
        i--;
        mon_cyl --;
    }
    lunar_month = i;
    lunar_day = offset + 1;
    char lunar_output[100] = {0};
    sprintf(lunar_output, "%s-%s-%s-%s-%s", lunar_tiangan_names[year_cyl%10], lunar_dizhi_names[year_cyl%12], is_leap == 1 ? "润" : "平", lunar_month_names[lunar_month], lunar_day_names[lunar_day]);
    char* ret = lunar_output; 
    RETVAL_STRINGL(lunar_output, strlen(ret));
/******************************↑******************************/
}

/******************************↓******************************/
PHP_MINIT_FUNCTION(lunar)
{
    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(lunar)
{
    return SUCCESS;
}
/******************************↑******************************/

PHP_RINIT_FUNCTION(lunar)
{
#if defined(ZTS) && defined(COMPILE_DL_LUNAR)
    ZEND_TSRMLS_CACHE_UPDATE();
#endif

    return SUCCESS;
}

/******************************↓******************************/
PHP_RSHUTDOWN_FUNCTION(lunar)
{
    return SUCCESS;
}
/******************************↑******************************/

PHP_MINFO_FUNCTION(lunar)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "lunar support", "enabled");
    php_info_print_table_end();
}

ZEND_BEGIN_ARG_INFO(arginfo_date_to_lunar, 0)
    ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

static const zend_function_entry lunar_functions[] = {
    PHP_FE(date_to_lunar, arginfo_date_to_lunar)
    PHP_FE_END
};

zend_module_entry lunar_module_entry = {
    STANDARD_MODULE_HEADER,
    "lunar",
    lunar_functions,
/******************************↓******************************/
    PHP_MINIT(lunar),
    PHP_MSHUTDOWN(lunar),
/******************************↑******************************/
    PHP_RINIT(lunar),
/******************************↓******************************/
    PHP_RSHUTDOWN(lunar),
/******************************↑******************************/
    PHP_MINFO(lunar),
    PHP_LUNAR_VERSION,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_LUNAR
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(lunar)
#endif

/*版权声明*/
/******************************↓******************************/
/******************************↑******************************/
/*之间的代码非本人原创，来源于*/
/*https://github.com/jixiaod/php-lunar-extension/blob/master/datetolunar.c*/