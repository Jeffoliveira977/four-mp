#pragma once

class HandleManager
{
public:
	HandleManager(void);
	~HandleManager(void);
	void ReleaseAllHandleTypes(const unsigned char pluginindex);
	bool RequestNewHandleType(const unsigned char pluginindex, unsigned short &typeindex);
	bool ReleaseHandleType(const unsigned char pluginindex, const unsigned short typeindex);
	void CloseAllHandles(const short owner);
	int AddNewHandle(const short owner, const unsigned short type, void *ptr);
	bool GetHandleType(const short owner, const int index, unsigned short &type);
	void *GetHandlePointer(const short owner, const int index);
	bool SetHandlePointer(const short owner, const int index, void *ptr);
	bool CloseHandle(const short owner, const int index);
	friend class CoreHandleTypesManager;
private:
	struct HandleType
	{
		unsigned char owner;
	};
	unsigned short maxtypebuffersize;
	unsigned short typebuffersize;
	HandleType **typebuffer;
	struct Handle
	{
		short numowners;
		short *owner;
		unsigned short type;
		void *ptr;
	};
	int maxhandlebuffersize;
	int handlebuffersize;
	Handle **handlebuffer;
	short maxhandlesperowner;
	short maxcountbuffersize;
	short countbuffersize;
	short **countbuffer;
	bool GetHandleTypeFreeSlot(unsigned short &index);
	bool ResizeHandleTypeBuffer(HandleType **&buffer, const unsigned short size);
	int GetHandleFreeSlot();
	int FindHandle(const void *ptr);
	bool IsHandleOwned(const int index, const short owner);
	bool AddHandleOwner(const int index, const short owner);
	bool DeleteHandleOwner(const int index, const short owner);
	bool ResizeHandleOwnerBuffer(short *&buffer, const short size);
	bool ResizeHandleBuffer(Handle **&buffer, const int size);
	bool IncreaseHandleCount(const short owner);
	bool DecreaseHandleCount(const short owner);
	bool ResizeHandleCountBuffer(short **&buffer, const short size);
};
