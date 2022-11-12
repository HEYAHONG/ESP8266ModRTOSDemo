#include "appcmd.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#define SCANF_BUFF_SIZE 128
#define SCANF_CMD_SIZE 16
#define MAX_CMD_STRING_ARRAY 5

typedef struct
{
    char *ptr;
    size_t offset;
    size_t length;
} cmd_string_t;

static void printf_help()
{
    printf("list of cmd:\n");
    printf("\twifi:\n\t\twifi ssid password\n");
    printf("\trestart:\n\t\trestart\n");
    printf("\texit:\n\t\texit\n");
}

//处理除exit restart之外的命令
static void run_cmd(cmd_string_t cmdarray[])
{

    if (strcasecmp(cmdarray[0].ptr, "wifi") == 0)
    {
        wifi_config_t *cfg = wifi_station_getconfig();
        if (cmdarray[1].length != 0 && cmdarray[2].length != 0)
        {
            memset(cfg, 0, sizeof(wifi_config_t));
            memcpy(cfg->sta.ssid, cmdarray[1].ptr, (cmdarray[1].length > sizeof(cfg->sta.ssid)) ? sizeof(cfg->sta.ssid) : cmdarray[1].length);
            memcpy(cfg->sta.password, cmdarray[2].ptr, (cmdarray[2].length > sizeof(cfg->sta.password)) ? sizeof(cfg->sta.password) : cmdarray[2].length);
            wifi_station_setconfig(cfg);
            printf("\nnew wifi info %s:%s\n", cfg->sta.ssid, cfg->sta.password);
        }
        else
        {
            printf("\ncurrent wifi info %s:%s\n", cfg->sta.ssid, cfg->sta.password);
            printf("\twifi:\n\t\twifi ssid password\n");
        }
    }

}

void enter_app_cmd()
{
    char  *buff = malloc(SCANF_BUFF_SIZE);
    char  *cmd = malloc(SCANF_CMD_SIZE);

    while (true)
    {
        memset(buff, 0, SCANF_BUFF_SIZE);
        memset(cmd, 0, SCANF_CMD_SIZE);
        printf_help();
        {
            int index = 0;
            printf(">");
            while (index < SCANF_BUFF_SIZE)
            {
                int ch = getchar_unlocked();
                if (ch > 0)
                {
                    if (ch == '\n' || ch == '\r')
                    {
                        if (index == 0)
                        {
                            printf("\n>");
                            continue;
                        }
                        else
                            break;
                    }
                    else
                    {
                        buff[index++] = ch;
                    }
                    putchar_unlocked(ch);
                }
                else
                {
                    vTaskDelay(10 / portTICK_PERIOD_MS);
                }
            }
        }
        printf("\nprocess cmd: %s\n", buff);
        cmd_string_t cmdarray[MAX_CMD_STRING_ARRAY] = {0}; //命令参数
        if (strlen(buff) > 0)
        {
            int cmdlen = 0;
            while (cmdlen  < SCANF_CMD_SIZE && cmdlen < strlen(buff) && buff[cmdlen] != '\r' && buff[cmdlen] != '\n'  && buff[cmdlen] != ' ')
            {
                cmdlen++;
            }
            if (cmdlen == 0)
            {
                continue;
            }
            memcpy(cmd, buff, cmdlen);
            if (strncasecmp(cmd, "exit", cmdlen) == 0)
            {
                break;
            }
            if (strncasecmp(cmd, "restart", cmdlen) == 0)
            {
                esp_restart();
            }
            cmdarray[0].ptr = cmd;
            cmdarray[0].offset = 0;
            cmdarray[0].length = cmdlen;
            size_t next_cmd_string = 1;
            for (int i = cmdlen; i < strlen(buff); i++)
            {
                if (buff[i] != ' ' &&  buff[i] != '\n')
                {
                    if (cmdarray[next_cmd_string].offset == 0)
                    {
                        cmdarray[next_cmd_string].ptr = &buff[i];
                        cmdarray[next_cmd_string].offset = i;
                        cmdarray[next_cmd_string].length = strlen(buff) - i;
                    }
                }
                else
                {
                    if (cmdarray[next_cmd_string].offset == 0)
                    {
                        continue;
                    }
                    cmdarray[next_cmd_string].length = i - cmdarray[next_cmd_string].offset;
                    if (next_cmd_string < MAX_CMD_STRING_ARRAY)
                    {
                        next_cmd_string++;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            run_cmd(cmdarray);
        }
    }
    free(buff);
    free(cmd);
}
