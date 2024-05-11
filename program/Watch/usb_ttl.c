#include "common.h"
#include "usart.h"

usb_ttl_data_t usb_data;

#define BUF_LEN 128
static u8 USART_DATA_LEN;
static int Connected = 0; //1 :connect ,0:initial ,-1:not connect

void get_cpu_from_buf(void);
void get_data_frombuf(void);
void get_time_data_frombuf(u8 t);
void get_cpu_data_frombuf(u8 t);
void get_weather_data_frombuf(u8 t);

void usb_ttl_init()
{
    int i = 0;
    for(i = 0;i<5;i++)
    {
        usb_data.weather[i].weather= malloc(3);
        memset(usb_data.weather[i].weather,0,3);
    }
}

void usb_ttl_exit()
{
}

int  usb_ttl_connected(void)
{
    static int time_con = -3;

    if(Connected)
    {
        time_con = timeDate.time.secs;
    }
    else 
    {
        if( timeDate.time.secs-time_con >= 3)
        {
            Connected = 0;
            time_con = -3;
            return 0;
        }
    }
    return 1;
}

void usb_ttl_update()
{
//    u16 len;
//    u8 true_t=0;
    if(USART_RX_STA&0x8000)
    {			
        USART_DATA_LEN=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
        get_data_frombuf();
       // printf(" USART_RX_BUF %s r\n", USART_RX_BUF);
        USART_RX_STA=0;
        memset(USART_RX_BUF,0,USART_DATA_LEN);
        // draw_string(USB_RX_BUF,false,0,20);
    }
    else
    {
        Connected = 0;
    }
}
static int net_speed_select = 0;
void get_data_frombuf()
{
    u8 t;
    // char str[6];
    for(t=0;t<USART_DATA_LEN;t++)
    {
        if(USART_RX_BUF[t]<32||USART_RX_BUF[t]>126)USART_RX_BUF[t]=32; // 32 = ' '
        get_time_data_frombuf(t);		
        get_cpu_data_frombuf(t);
        get_weather_data_frombuf(t);
    }
    // printf("Connected %d\r\n",Connected);	
    net_speed_select = 0;
}
void get_cpu_data_frombuf(u8 t)
{
    u8 i=0;
    char str[6] = {0,0,0,0,0,0};
    //static int cpu_bug = 12; /*有时候会收到CPU利用率 = 12，这个是串口发送软件问题，不是接收问题*/
    if(USART_RX_BUF[t]=='C' && USART_RX_BUF[t+1]==':')
    {
        str[0]=USART_RX_BUF[t+2]; 
        str[1]=USART_RX_BUF[t+3]; 
        usb_data.cpu = atoi(str);
        
        // printf("%d\n",usb_data.cpu);
    }
    if(USART_RX_BUF[t]=='M'&& USART_RX_BUF[t+1]==':')
    {
        str[0]=USART_RX_BUF[t+2]; 
        str[1]=USART_RX_BUF[t+3]; 
        usb_data.member = atoi(str);
    }

    // if(USART_RX_BUF[t]=='T')
    // {
    //     str[0]=USART_RX_BUF[t+2]; 
    //     str[1]=USART_RX_BUF[t+3]; 
    //     usb_data.temp = atoi(str);
    //     memset(str,0,6);
    // }
    // if(USART_RX_BUF[t]=='n')
    // {
    //     while(USART_RX_BUF[t+2+i]<='9'&&USART_RX_BUF[t+2+i]>='0')
    //     {
    //         str[i]=USART_RX_BUF[t+2+i]; 
    //         i++;
    //         if(i==5)break;
    //     }
    //     net_speed_select =  atoi(str)>net_speed_select?atoi(str):net_speed_select;
    //     usb_data.netspeed = net_speed_select;

    //     memset(str,0,6);
    // }
    // if(USART_RX_BUF[t]=='p')
    // {
    //     str[0]=USART_RX_BUF[t+2]; 
    //     str[1]=USART_RX_BUF[t+3]; 
    //     usb_data.tdp = atoi(str);
    // }
}
void get_time_data_frombuf(u8 t)
{
    u8 sec_tmp=0;
    char str[3];
    if(USART_RX_BUF[t]=='d'&&USART_RX_BUF[t+1]==':')
    {
        str[0]=USART_RX_BUF[t+4];
        str[1]=USART_RX_BUF[t+5];
        timeDate.date.year = atoi(str);

        str[0]=USART_RX_BUF[t+7];
        str[1]=USART_RX_BUF[t+8];
        timeDate.date.month = (month_t)(atoi(str)-1);

        str[0]=USART_RX_BUF[t+10];
        str[1]=USART_RX_BUF[t+11];
        timeDate.date.date = (day_t)atoi(str);

        str[0]=USART_RX_BUF[t+13]; 
        str[1]=USART_RX_BUF[t+14]; 
        timeDate.time.hour = atoi(str);
        str[0]=USART_RX_BUF[t+16]; 
        str[1]=USART_RX_BUF[t+17]; 
        timeDate.time.mins = atoi(str);
        str[0]=USART_RX_BUF[t+19]; 
        str[1]=USART_RX_BUF[t+20]; 
        sec_tmp = atoi(str);
        if( sec_tmp-timeDate.time.secs > 10)
            timeDate.time.secs = sec_tmp;

        str[0]=USART_RX_BUF[t+22]; 
        str[1]=USART_RX_BUF[t+23]; 
        str[2]=USART_RX_BUF[t+24];

        // printf("str = %s\r\n",str);
        if(strcmp(str,STR_MON) == 0) timeDate.date.day = 0;
        else if(strcmp(str,STR_TUE) == 0) timeDate.date.day = 1;
        else if(strcmp(str,STR_WED) == 0) timeDate.date.day = 2;
        else if(strcmp(str,STR_THU) == 0) timeDate.date.day = 3;
        else if(strcmp(str,STR_FRI) == 0) timeDate.date.day = 4;
        else if(strcmp(str,STR_SAT) == 0) timeDate.date.day = 5;
        else if(strcmp(str,STR_SUN) == 0) timeDate.date.day = 6;
        // printf("timeDate.date.day = %d\r\n",timeDate.date.day);

         Connected = 1;
    }	
}
void get_weather_data_frombuf(u8 t)
{
    int buf_pos = 0;
    char str[3] ={0,0,0};
    char *buf;
    int buf_i = 0;
  
    /*
        W:clo-15℃~-9℃]ove-15℃~-10℃]ove-16℃~-8℃]ove-15℃~-8℃]ove-16℃~-6℃]
        W:sno-6℃~0℃]sun-7℃~-1℃]clo-7℃~0℃]sun-6℃~0℃]ove-5℃~0℃]
        W:rai3℃~11℃]sun5℃~7℃]rai4℃~8℃]ove4℃~6℃]ove4℃~7℃]
    */
    if(USART_RX_BUF[t]=='W' && USART_RX_BUF[t+1]==':')
    {
        buf = malloc(80);
        memset(buf,0,80);
        strcpy(buf,&USART_RX_BUF[t+2]);
        while(buf_pos < 5)
        {
            //memset(usb_data.weather[buf_pos].weather,0,3);
            memcpy(usb_data.weather[buf_pos].weather,buf+buf_i,3);
            printf("weather:%s#len = %d\r\n",usb_data.weather[buf_pos].weather,strlen(usb_data.weather[buf_pos].weather));
            buf_i += 3;
            memcpy(str,buf+buf_i,3);
            usb_data.weather[buf_pos].temp_low = atoi(str);
            buf_i += 2;
            while(buf[buf_i] != '~')
            {
                buf_i++;
            }
            buf_i++;
            memcpy(str,buf+buf_i,3);
            usb_data.weather[buf_pos].temp_high = atoi(str);
            buf_i += 2;

            while(buf[buf_i] != ']')
            {
                buf_i++;
            }
            buf_i++;
            weather_type_set(&usb_data.weather[buf_pos]);
            printf("weather_type:%d\r\n",usb_data.weather[buf_pos].weather_type);
            buf_pos++;
        }
        free(buf);
        // while(buf_pos<5)
        // {
        //     str[0]=USART_RX_BUF[t+2+11*buf_pos]; 
        //     str[1]=USART_RX_BUF[t+3+11*buf_pos]; 
        //     str[2]=USART_RX_BUF[t+4+11*buf_pos]; 
        //     strcpy(usb_data.weather[buf_pos].weather,str);
        //     // str[2] = 0;

        //     str[0]=USART_RX_BUF[t+5+11*buf_pos]; 
        //     str[1]=USART_RX_BUF[t+6+11*buf_pos];
        //     str[2]=USART_RX_BUF[t+7+11*buf_pos];  
        //     usb_data.weather[buf_pos].temp_low = atoi(str);

        //     if(USART_RX_BUF[t+9+11*buf_pos] == '~')
        //     {
        //         str[0]=USART_RX_BUF[t+10+11*buf_pos]; 
        //         str[1]=USART_RX_BUF[t+11+11*buf_pos]; 
        //         str[2]=USART_RX_BUF[t+12+11*buf_pos]; 
        //         usb_data.weather[buf_pos].temp_high = atoi(str);
        //     }
        //     else
        //     {
        //         str[0]=USART_RX_BUF[t+9+11*buf_pos]; 
        //         str[1]=USART_RX_BUF[t+10+11*buf_pos]; 
        //         str[2]=USART_RX_BUF[t+11+11*buf_pos]; 
        //         usb_data.weather[buf_pos].temp_high = atoi(str);
        //     }

        //     weather_type_set(&usb_data.weather[buf_pos]);
        //     // printf("1 %s\n",usb_data.weather[buf_pos].weather);
        //     // printf("2 %d\n",usb_data.weather[buf_pos].temp_low);
        //     // printf("3 %d\n",usb_data.weather[buf_pos].temp_high);
        //     buf_pos++;
        // }
        // printf("%d\n",usb_data.cpu);
    }
}
int put_cpu(u8 i)
{
    int res;
    switch (i)
    {
    case 0: 
        res = usb_data.cpu;
        break;
    case 1: 
        res = usb_data.member;
        break;
    case 2: 
        res = usb_data.temp;
        break;
    case 3: 
        res = usb_data.netspeed;
        break;
    case 4: 
        res = usb_data.tdp;
        break;
    default:
        break;
    }
    //if(res>99)res=99;
    return res;
}
usb_ttl_weather_t get_weather(u8 num)
{
    return usb_data.weather[num];
}