#include <gtk-2.0/gtk/gtk.h>  
#include <gtk-2.0/gdk/gdk.h>  
#include <gtk-2.0/gtk/gtkdialog.h>  
#include <gtk-2.0/gtk/gtktypeutils.h>  
#include <gtk-2.0/gtk/gtkmain.h>  
#include <gtk-2.0/gtk/gtkstyle.h>  
#include <gtk-2.0/gtk/gtkmessagedialog.h>  
#include <gtk-2.0/gtk/gtkwindow.h>  
#include "stdio.h"
#include <string.h>
#include "stdlib.h"
#include <curl/curl.h>
#include "iostream"
using namespace std;  
  
  GtkWidget *psnView;  
	GtkWidget *VIPcode;
	GtkWidget *window;  
	GtkWidget *hbox;
	GtkWidget *vbox;
	void query(char * psn);
	 void login(void);
 void upload(char * psn,char * vip_code);
int query_OK_flag = 0;
int login_OK_flag = 0;
int upload_OK_flag = 0;
void getvip(GtkWidget *widget, gpointer entry)
{
   
   
   const gchar *entry_text;  
   char psn[50]= {0};
		//获取psn列表 = {}
		
    FILE *fp;
	int fileLen;
	 char *fileContent = NULL;
	fp = fopen("OTA_list.log","rb");
	if(fp == NULL)
	{
		printf("无法打开PSN文件列表\n");
		return;
	}
	
/*	fseek(fp,0,SEEK_END);
	fileLen = ftell(fp);
	fseek(fp,0,SEEK_SET);
	fread(fileContent,1,fileLen,fp);
*/
	size_t len;
	
        //获取文本内容  
        entry_text = gtk_entry_get_text(GTK_ENTRY(psnView));  
		memcpy(psn,entry_text,8);
        printf("psn:%s\n", psn);
		//string psn(entry_text);		
		//string res(fileContent);
		//printf("%d\n",res.find(psn));
		int psnMatchedFlag = 0;
		while(getline(&fileContent,&len,fp)!=-1)
		{
			
			if(strncmp(fileContent,entry_text,8) == 0)
			{
				psnMatchedFlag = 1;
			printf("%s匹配\n",fileContent);
			fileContent[8] = '\0';
			query(fileContent);
			gtk_window_set_focus(GTK_WINDOW(window),VIPcode);
		
			break;
			}
		}
		fclose(fp);
		if(psnMatchedFlag != 1)
		{
			printf("\033[1;31;46mpsn未通过验证\033[0m\n");
			gtk_window_set_focus(GTK_WINDOW(window),psnView);
			return;
		}
		
 //const gchar *entry_text;  
  //printf("running\n");
        //获取文本内容  
        entry_text = gtk_entry_get_text(GTK_ENTRY(entry));  
  
        printf("vip:%s\n",entry_text);  
		gtk_window_set_focus(GTK_WINDOW(window),psnView);

		
		FILE * vipList;
		vipList = fopen("vip.list","a+");
		if(vipList == NULL)
		{
			printf("无法打开vip.list\n");
		}
		char vip_psn[100];
		sprintf(vip_psn,"%s %s\n",psn,entry_text);
		char vip_code[12];
		memcpy(vip_code,entry_text,11);
		fwrite(vip_psn,1,strlen(vip_psn),vipList);
		psn[8] = '\0';
		upload(psn,vip_code);
		fclose(vipList);
        return;  
   
}
void getpsn(GtkWidget *widget, gpointer entry)
{
   #if 0
 const gchar *entry_text;  
  
		//获取psn列表
    FILE *fp;
	int fileLen;
	 char *fileContent;
	fp = fopen("OTA_list.log","rb");
	vipList = fopen("vip.log","rb");
	if(fp == NULL)
	{
		printf("无法打开PSN文件列表\n");
		return;
	}
	
/*	fseek(fp,0,SEEK_END);
	fileLen = ftell(fp);
	fseek(fp,0,SEEK_SET);
	fread(fileContent,1,fileLen,fp);
*/
	size_t len;
	
        //获取文本内容  
        entry_text = gtk_entry_get_text(GTK_ENTRY(entry));  
		
        printf("psn:%s\n", entry_text);
		//string psn(entry_text);		
		//string res(fileContent);
		//printf("%d\n",res.find(psn));
		int psnMatchedFlag = 0;
		while(getline(&fileContent,&len,fp)!=-1)
		{
			if(strncmp(fileContent,entry_text,8) == 0)
			{
				psnMatchedFlag = 1;
			printf("%s匹配\n",fileContent);
			gtk_window_set_focus(GTK_WINDOW(window),VIPcode);
			break;
			}
		}
		if(psnMatchedFlag != 1)
		{
			printf("psn未通过验证\n");
		}
		
#endif
gtk_window_set_focus(GTK_WINDOW(window),VIPcode);
        return;  
   
}
//使用gtk编译参数加上 `pkg-config --cflags --libs gtk+-2.0`  
 
int main(int argc, char** argv) {  
  
	
	printf("%s\n",curl_version());
	GtkTextBuffer *buffer;
	login();
	sleep(2);
	
    //初始化GTK环境      
    gtk_init(&argc, &argv); 
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	
	gtk_widget_set_size_request(window,200,250);
	
	psnView = gtk_entry_new();//创建文本框
	
	VIPcode = gtk_entry_new();//创建文本框
	
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	
	g_signal_connect(psnView, "activate", G_CALLBACK(getpsn), psnView);
	
	g_signal_connect(VIPcode, "activate", G_CALLBACK(getvip), VIPcode);
	
	gtk_entry_set_max_length(GTK_ENTRY(psnView), 8);  
	
	vbox = gtk_vbox_new(FALSE,0);
	gtk_container_add(GTK_CONTAINER(window),vbox);

	hbox = gtk_hbox_new(FALSE,0);
	gtk_box_pack_end(GTK_BOX(vbox),hbox,TRUE,TRUE,0);
	
	gtk_box_pack_end(GTK_BOX(vbox),VIPcode,TRUE,TRUE,0);
	gtk_box_pack_end(GTK_BOX(vbox),psnView,TRUE,TRUE,0);
	//gtk_container_add(GTK_CONTAINER(fixed),psnView);
	//gtk_box_pack_end(GTK_BOX(hbox),psnView,TRUE,TRUE,0);
	//gtk_fixed_put(GTK_FIXED(fixed), VIPcode, 50, 50);//函数将widget放在fixed的由x和y指定的位置。
	//gtk_container_add(GTK_CONTAINER(window),VIPcode);
	
	gtk_widget_show(psnView);
	
	gtk_widget_show(VIPcode);
	gtk_widget_show(vbox);
	gtk_widget_show(hbox);
	gtk_widget_show(window);
	
	gtk_main();
    return 0;  
}  

size_t login_Log(void *ptr, size_t size, size_t nmemb, void *stream)
{
	size_t size_read = 0;
	//size_read = fwrite(ptr, size, nmemb, (FILE*)stream);
	cout << (char*)ptr << endl;
	login_OK_flag = 0;
	if((strstr((const char *)ptr, (const char *)"\"code\":0") != NULL))
	{
		login_OK_flag = 1; 
		printf("登陆成功\n");
	}
	else //if((strstr((const char *)ptr, (const char *)"\"code\":200") != NULL))
	{
		login_OK_flag = 0;
		printf("登陆失败\n");
	}
	
	return size_read;
}
size_t upload_Log(void *ptr, size_t size, size_t nmemb, void *stream)
{
	size_t size_read = 0;
	//size_read = fwrite(ptr, size, nmemb, (FILE*)stream);
	cout << (char*)ptr << endl;
	upload_OK_flag = 0;
	if((strstr((const char *)ptr, (const char *)"\"code\":0") != NULL))
	{
		printf("设备已绑定成功\n");
		upload_OK_flag = 1;
	}
	else //if((strstr((const char *)ptr, (const char *)"\"code\":200") != NULL))
	{
		printf("设备绑定失败\n");
		upload_OK_flag = 0;
	}
	return size_read;
}
size_t query_Log(void *ptr, size_t size, size_t nmemb, void *stream)
{
	size_t size_read = 0;
	//size_read = fwrite(ptr, size, nmemb, (FILE*)stream);
	cout << (char*)ptr << endl;
	query_OK_flag = 0;
	if((strstr((const char *)ptr, (const char *)"\"code\":0") != NULL))
	{
		printf("设备已添加\n");
		query_OK_flag = 1;
	}
	else if((strstr((const char *)ptr, (const char *)"\"code\":200") != NULL))
	{
		printf("设备未添加\n");
		query_OK_flag = 0;
	}
	return size_read;
}
void login(void)
{
	CURL* curl = curl_easy_init();
	if (NULL == curl)
	{
		return ;
	}
	curl_easy_setopt(curl, CURLOPT_URL, "http://moss.internal.extantfuture.com/login/loginAction.do");
	curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "cookie.txt");
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "userAccount=yanan&userPassword=E10ADC3949BA59ABBE56E057F20F883E");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, login_Log);
	//curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	return ;
}

void upload(char * psn,char * vip_code)
{
	CURL* curl = curl_easy_init();
	if (NULL == curl)
	{
		return ;
	}
	char post[500];
	sprintf(post,"psn=%s&hardwareEdition=10&controlBootloader=34&calcBootloader=35&controlVersion=2111&calcVersion=1954&deviceVersion=281479271743489&qrCode=%s&cmd=5",psn,vip_code);
	curl_easy_setopt(curl, CURLOPT_URL, "http://moss.internal.extantfuture.com/device/bootloaderEditionAction.do");
	curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "cookie.txt");
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post);
	printf("%s\n",post);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, upload_Log);
	//curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	return ;
}
void query(char * psn)
{
	CURL* curl = curl_easy_init();
	char post[500];
	sprintf(post,"cmd=1&psn=%s&page=1&pageSize=10",psn);
	if (NULL == curl)
	{
		return ;
	}
	curl_easy_setopt(curl, CURLOPT_URL, "http://moss.internal.extantfuture.com/device/bootloaderEditionAction.do");
	curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "cookie.txt");
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post);
	printf("%s\n",post);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, query_Log);
	//curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	return ;
}