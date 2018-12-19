#ifndef __CZHTFACE_INTERFACE_H__
#define __CZHTFACE_INTERFACE_H__

#include <interface.h>
#include "faceDetectInterface.h"

using namespace std;
class CCzhtFaceInterface : public CFaceDetectInterface
{
public:
	CCzhtFaceInterface();
	~CCzhtFaceInterface();
	
public:
	virtual int				rzt_face_geterror();

	virtual bool			rzt_face_init(IN char *pszSvrAddr, IN char *pszSvrPort);
	virtual bool			rzt_face_uninit();
	
	virtual bool			rzt_face_image_insert(IN char *pszFaceDbName, IN char *pszFaceImage, IN FACE_USERDATA &UserData);
	virtual bool			rzt_face_image_search(IN char *pszFaceDbName, IN char *pszFaceImage, OUT char **pszFaceSearchInfo);

	virtual bool			rzt_face_getuserdata(OUT std::map<int, FACE_USERRESULT> &mapFaceResult);
};

#endif