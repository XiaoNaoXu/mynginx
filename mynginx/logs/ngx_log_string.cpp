#include <string.h>
#include <string>
#include <stdarg.h>

#include "ngx_log.h"

#include <iostream>
using namespace std;

typedef struct {
        int x;
        int X;
        int u;
        int point;
}spe_type;

u_int ngx_int_to_buf(char *buf, 
                                                int var, int b_x, 
                                                int s_x, 
                                                u_int add_num, 
                                                u_int i_width, 
                                                char c_temp)
{
        char temp_buf[NGX_BUFF_MAXSIZE];      //临时存储
        char hex[] = "0123456789abcdef";      //小写16进制数组
        char HEX[] = "0123456789ABCDEF";      //大写16进制数组
        int count = 0;  //计数量
        u_int remainder = 0;    //余数存储
        if(s_x == 1)
        {
                do
                {
                        remainder = var%16;
                        temp_buf[count ++] = hex[remainder];
                }while ((var /= 16 ) > 0);
        }
        else if (b_x == 1)
        {
                do
                {
                        remainder = var%16;
                        temp_buf[count ++] = HEX[remainder];
                }while ((var /= 16 ) > 0);
        }
        else
        {
                do
                {
                        temp_buf[count++] = var%10 + '0';
                }while((var /= 10) >= 1);
        }
        
        if(i_width > 0)
        {
                while(count != i_width--)
                {
                        buf[add_num++] = c_temp;
                }
        }
        temp_buf[count--] = '\0';
        while(count > -1)
        {
                buf[add_num++] = temp_buf[count--];
        }
        return add_num;
}

u_int ngx_float_to_buf(char *buf,
                                                double f_var,
                                                u_int point,
                                                u_int add_num,
                                                int i_width
                                                )
{
        //
        u_int temp_buf[NGX_BUFF_MAXSIZE];       //临时存储BUFF
        u_int count = 0;                                                        //计数器
        int integer = f_var;
        double decimals = f_var - (double)integer + 0.000000000001;

        //追加整数部分
        while(integer > 0)
        {
                temp_buf[count ++] = integer%10 + '0';
                integer /= 10;
        }
        while (count--)
        {
                buf[add_num++] = temp_buf[count];
        }
        
        //加入小数点
        buf[add_num++] = '.';
        //追加小数部分
        if(point == 1)
        {
                while(decimals  < 1 && decimals > 0.000001  && i_width--)
                {
                        buf[add_num++] = (int)(decimals * 10) + '0'; 
                        decimals =  (double)(decimals * 10)  - (int)(decimals * 10);
                }
                while(i_width-- && i_width > 0)
                {
                        buf[add_num++] = '0';
                }
        }
        else
        {
                i_width = 6;
                while(decimals  < 1 && decimals > 0.000001 && i_width--)
                {
                        if(!i_width)
                        {
                                //
                        }
                        buf[add_num++] = (int)(decimals * 10) + '0'; 
                        decimals =  (double)(decimals * 10)  - (int)(decimals * 10);       
                }
                while(i_width-- && i_width > 0)
                {
                        buf[add_num++] = '0';
                }
        }
        
        
        return add_num;
}

//组合信息到BUFF里面
u_int ngx_printf_to_buf(char *buf, const char *fmt, va_list v_argv)
{
        u_int add_num = 0;      //追加字符数量
        u_int i_width = 0;              //规定显示宽度
        char *s_var ;                           //可变参为字符串
        int i_var = 0;                        //可变参为整数
        char c_char;                          //可变参为字符
        double d_var = 0.0;
        spe_type stype{NGX_CHAR_N , 
                                        NGX_CHAR_N , 
                                        NGX_CHAR_N, 
                                        NGX_CHAR_N};
        /*if(buf[add_num] == '\0')
        {
                buf[add_num] = ' ';
        }*/
        while(*fmt != '\0' )
        {
                if(*fmt == '%')
                {
                        char temp = (*((fmt++) + 1)) == '0' ? '0':' ';
                        stype.x = NGX_CHAR_N;
                        stype.X = NGX_CHAR_N;
                        stype.u = NGX_CHAR_N;
                        stype.point = NGX_CHAR_N;
                        i_width = 0;
                        while(true)
                        {
                                //参数为数字
                                if(*fmt - '0' > -1 && *fmt - '0' < 10)
                                {
                                        while(*fmt >= '0' && *fmt <= '9')
                                        {
                                                i_width = 10 * i_width + (*fmt++ - '0');
                                        }
                                }
                                //参数为小写x
                                else if(*fmt == 'x')
                                {
                                        stype.x = NGX_CHAR_Y; ++fmt; 
                                }
                                //参数为大写X
                                else if(*fmt == 'X')
                                {
                                        stype.X = NGX_CHAR_Y; ++fmt;
                                }
                                //参数为无符号u
                                else if(*fmt == 'u')
                                {
                                        stype.u = NGX_CHAR_Y;  ++fmt;
                                }
                                else if(*fmt == '.')
                                {
                                        stype.point = NGX_CHAR_Y;
                                        while(*fmt++ >= '0' && *fmt++ <= '9')
                                        {
                                                i_width = 10 * i_width + (*fmt++ - '0');
                                        }
                                }
                                else
                                {
                                        break;
                                }
                        }
                        switch(*fmt++)
                        {
                                case 'd': 
                                        if(stype.u == NGX_CHAR_Y)
                                        {
                                                i_var = va_arg(v_argv, int);
                                        }
                                        else
                                        {
                                                int var = va_arg(v_argv, int);
                                                if(var < 0)
                                                {
                                                        buf[add_num++] = '-';
                                                        i_var = (u_int)var;
                                                }
                                                else
                                                {
                                                        i_var = var;
                                                }        
                                        }
                                        add_num = ngx_int_to_buf(buf, i_var, 
                                                                                                stype.X, stype.x,
                                                                                                add_num, i_width, 
                                                                                                temp);
                                        break;
                                case 's':
                                         s_var = va_arg(v_argv, char *);
                                        if(i_width > 0)
                                        {
                                                while(i_width-- != strlen(s_var))
                                                {
                                                        buf[add_num++] = temp;
                                                }
                                        }
                                        while(*s_var != '\0')
                                        {
                                                buf[add_num++] = *s_var++;
                                        }
                                        continue;
                                case 'f':
                                        d_var = va_arg(v_argv, double);
                                         add_num = ngx_float_to_buf(buf,d_var,
                                                                                                        stype.point,
                                                                                                        add_num, i_width);
                                        
                                        break;
                                case 'c':
                                        c_char = (char)va_arg(v_argv, int);
                                        if(i_width > 0)
                                        {
                                                while(i_width-- != 1)
                                                {
                                                        buf[add_num++] = temp;
                                                }
                                        }
                                        buf[add_num++] = c_char;
                                        break;
                                default:
                                        buf[add_num++] = *fmt;
                                        continue;
                        }
                }
                else
                {
                        buf[add_num++] = *fmt++;
                }
        }
        return add_num;
}


u_int ngx_time_to_buf(char *buf, const char *fmt, ...)
{
        va_list v_valist;
        va_start(v_valist, fmt);
        int temp_int = ngx_printf_to_buf(buf, fmt, v_valist);
        va_end(v_valist);
        return temp_int;
}