#ifndef __FACEDETECT_INTERFACE_H__
#define __FACEDETECT_INTERFACE_H__

#define FACE_INIT_FAILED		0x00000001
#define FACE_DETECT_FAILED		0x00000002
#define FACE_INSERTDB_FAILED	0x00000003
#define FACE_INSERT_TIMEOUT		0x00000004
#define FACE_DATA_ERROR			0x00000005

#define MAX_USERCHAR_LEN				32
typedef struct tagUserResult
{
	int			iFaceValue;
	char		szUserId[MAX_USERCHAR_LEN];
}FACE_USERRESULT;

typedef struct tagUserDATA
{
	char		szUserId[MAX_USERCHAR_LEN];
	char		szUserName[MAX_USERCHAR_LEN];
	char		szUserDep[MAX_USERCHAR_LEN];
}FACE_USERDATA;

using namespace std;
class CFaceDetectInterface
{
public:
	virtual int				rzt_face_geterror() = 0;
	
	virtual bool			rzt_face_init(IN char *pszSvrAddr, IN char *pszSvrPort) = 0;
	virtual bool			rzt_face_uninit() = 0;
	
	virtual bool			rzt_face_image_insert(IN char *pszFaceDbName, IN char *pszFaceImage, IN FACE_USERDATA &UserData) = 0;
	virtual bool			rzt_face_image_search(IN char *pszFaceDbName, IN char *pszFaceImage, OUT char **pszFaceSearchInfo) = 0;

	virtual bool			rzt_face_getuserdata(OUT std::map<int, FACE_USERRESULT> &mapFaceResult) = 0;
	
protected:
	int						m_ierror;
	std::map<int, FACE_USERRESULT> m_mapFaceResult;
};

#endif