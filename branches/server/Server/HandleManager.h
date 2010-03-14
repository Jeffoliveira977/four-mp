#pragma once

#define INVALID_HANDLE -1

class HandleManager
{
public:
	HandleManager(void);
	~HandleManager(void);
	void ReleaseAllHandleTypes(const unsigned char pluginindex); //For C++ plugins. Releases all handle types reserved by plugin
	bool RequestNewHandleType(const unsigned char pluginindex, unsigned short &typeindex); //For C++ plugins. Reserves empty handle type and returns it to plugin
	bool ReleaseHandleType(const unsigned char pluginindex, const unsigned short typeindex); //For C++ plugins. Release handle type reserved by plugin
	void CloseAllHandles(const short owner); //Closes all handles that was created or reserved by plugin or filterscript
	int AddNewHandle(const short owner, const unsigned short type, void *ptr); //Adds new handle to the handle buffer and returns it's index
	bool GetHandleType(const short owner, const int index, unsigned short &type); //Returns handle type given it's index
	void *GetHandlePointer(const short owner, const int index); //Returns pointer to data which given handle points to
	bool SetHandlePointer(const short owner, const int index, void *ptr); //Sets pointer to data which given handle points to
	bool CloseHandle(const short owner, const int index); //Closes handle which was created or reserved by plugin or filterscript
	friend class CoreHandleTypesManager;
private:
	struct HandleType
	{
		unsigned char owner; //Since only plugins can reserve handle types, this is equal to the plugin index
	};
	unsigned short maxtypebuffersize; //Holds maximum size of the handle *types* buffer
	unsigned short typebuffersize; //Holds current size of the handle *types* buffer
	HandleType **typebuffer; //Holds all handle types
	short pluginowneroffset; //Holds offset of plugins in the handle owner index
	struct Handle
	{
		short numowners; //Holds number of owners of the current handle
		short *owner; //Holds all current handle owners. 0 - core, 1 - game mode, 2..pluginowneroffset - 1 - filterscripts, pluginowneroffset..maxcountbuffersize - 1 - plugins
		unsigned short type; //Holds current handle type
		void *ptr; //Holds pointer to the data current handle points to
	};
	int maxhandlebuffersize; //Holds maximum size of the handle buffer
	int handlebuffersize; //Holds current size of the handle buffer
	Handle **handlebuffer; //Holds all handles
	short maxhandlesperowner; //Holds maximum number of handles that each owner can have
	short maxcountbuffersize; //Holds maximum size of handle *count* buffer
	short countbuffersize; //Holds current size of handle *count* buffer
	short **countbuffer; //Holds number of handles each owner has reserved
	bool GetHandleTypeFreeSlot(unsigned short &index); //Returns free slot in handle *types* buffer
	bool ResizeHandleTypeBuffer(HandleType **&buffer, const unsigned short size); //Wrapper for realloc
	int GetHandleFreeSlot(); //Returns free slot in handle buffer
	int FindHandle(const void *ptr); //Returns handle index given the pointer it points to
	bool IsHandleOwned(const int index, const short owner); //Checks if given handle is owned by given owner
	bool AddHandleOwner(const int index, const short owner); //Adds new owner to the given handle
	bool DeleteHandleOwner(const int index, const short owner); //Deletes given owner from given handle
	bool ResizeHandleOwnerBuffer(short *&buffer, const short size); //Wrapper for realloc
	bool ResizeHandleBuffer(Handle **&buffer, const int size); //Wrapper for realloc
	bool IncreaseHandleCount(const short owner); //Increases number of handles reserved by given owner by one
	bool DecreaseHandleCount(const short owner); //Decreases number of handles reserved by given owner by one
	bool ResizeHandleCountBuffer(short **&buffer, const short size); //Wrapper for realloc
};
