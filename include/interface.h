#ifndef _CZHT_FACE_DETECT_H_
#define _CZHT_FACE_DETECT_H_

#ifdef CZHTFACEDETECTDLL_EXPORTS
#define CZHTFACEDETECTDLL_API __declspec(dllexport) 
#else
#ifdef __linux__
#define CZHTFACEDETECTDLL_API  
#else
#define CZHTFACEDETECTDLL_API __declspec(dllimport) 
#endif
#endif

#define   IN
#define   OUT
#define   INOUT 

#define   USER_INFO_CHAR_LEN     32
typedef struct _stUserInfo
{	
	char    userId[USER_INFO_CHAR_LEN];        // 用户账号，必选，不能为空
	char    userName[USER_INFO_CHAR_LEN];      // 用户姓名，可选，可以为空
	char    userDep[USER_INFO_CHAR_LEN];       // 用户部门，可选，可以为空
}stUserInfo;

/*
 函数功能:    人脸识别SDK初始化。
 函数参数:    IN char *master_ip     主用后台服务IP 
 			  IN char *backup_ip     备用后台服务IP, 没有备用时，该字段赋值为NULL
 			  IN char *port          后台服务端口号
 函数返回值:  0：成功；非0：失败。
*/
CZHTFACEDETECTDLL_API int czht_face_init(IN char *master_ip, IN char *backup_ip, IN char *port);

/*
 函数功能:    向搜索库中插入一张图片。
 函数参数:    IN char* face_image_db_name  人脸库名称， 如果字符串为空，则表示不指定人脸库
              IN char* face_image_name     图片名，包含绝对路径。
              IN stUserInfo& user_info     图片对应的用户信息。
 函数返回值:  0：成功； 非0：失败（其中, -1：人脸检测失败, -2： 插入超时,  -3：数据库插入失败)
*/
CZHTFACEDETECTDLL_API int czht_face_image_insert(IN char* face_image_db_name, IN char* face_image_name, IN stUserInfo& user_info);

/*
 函数功能:    输入一张图片，返回搜索库中与该图片最相似的图片，最多10张，score值大的排在前面。
 函数参数:    IN  char* face_image_db_name   人脸库名称， 如果字符串为空，则表示不指定人脸库
              IN  char* face_image_name      图片名，包含绝对路径。
              OUT char* face_search_result   返回搜索结果，格式为json:
                字段说明：
                字段	            类型	     说明
                status	          string	   搜索结果状态
                score	            float	     搜索出的相似的图片的得分
                userId            string     用户账号
       示例：
                {
                  "status": "OK",
                  "result": 
                  	[
                  		{"score": "xx", "userId": "xx"},
                  		{"score": "xx", "userId": "xx"},
                  		{"score": "xx", "userId": "xx"}
                  	]
                }
              
 函数返回值:  0：成功； 非0：失败（其中, -1：人脸检测失败  -2： 超时或数据异常)
*/
CZHTFACEDETECTDLL_API int czht_face_image_search(IN char* face_image_db_name, IN char* face_image_name, OUT char** face_search_result);

#endif // _CZHT_FACE_DETECT_H_
