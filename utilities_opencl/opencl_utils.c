/* 
Author: Jared Laufiso 
Date:	8/4/2017

Collection of tools to make the openCL design process easier. 
*/


// The CHECK_STATUS macro 
// Purpose: to check the error code enums and determine if an error has occured.
// Preconditions: 	The error code returned from an OpenCL API function is passed as an 
// 					cl_int.
// Postcondtions:	If there has been an error, an error message will be printed to the user
// 					stating where the error occured aswell as the openCL error that occured. 
// note: This has been implemented as a macro, because this function is called for every API 
// 		 call. The macro may allow the compiler to optimize the program.  
#define CHECK_STATUS(status) \
		if(status != CL_SUCCESS){\
			fprintf(stderr,\
				"OpenCL error in file %s line %d, error code %s\n", \
					__FILE__, __LINE__, \
					opencl_error_to_str(status)); \
			exit(0); \
		}


// The opencl_error_to_str method 
// Purpose:	Dictionary to return the string literal value of the const enum 
// 			opencl error message.
// Preconditions: The macro "#define CASE_CL_ERROR(NAME) case NAME: return #NAME" has been defined 
// 				  The cl_int enum value of error message.
// Postcondtions: returns the string litteral value of the opencl err enum.  
#define CASE_CL_ERROR(NAME) case NAME: return #NAME
const char* opencl_error_to_str(cl_int error) {
	switch(error) {
		CASE_CL_ERROR(CL_SUCCESS);
		CASE_CL_ERROR(CL_DEVICE_NOT_FOUND);
		CASE_CL_ERROR(CL_DEVICE_NOT_AVAILABLE);
		CASE_CL_ERROR(CL_INVALID_VALUE);
		CASE_CL_ERROR(CL_OUT_OF_HOST_MEMORY);
		CASE_CL_ERROR(CL_INVALID_PLATFORM);
		CASE_CL_ERROR(CL_INVALID_PROPERTY);
		CASE_CL_ERROR(CL_INVALID_DEVICE);
		CASE_CL_ERROR(CL_INVALID_OPERATION);
		CASE_CL_ERROR(CL_INVALID_PROGRAM);
		CASE_CL_ERROR(CL_INVALID_PROGRAM_EXECUTABLE);
		CASE_CL_ERROR(CL_INVALID_KERNEL_NAME);
		CASE_CL_ERROR(CL_INVALID_KERNEL_DEFINITION); 	
		CASE_CL_ERROR(CL_INVALID_CONTEXT);
		CASE_CL_ERROR(CL_INVALID_QUEUE_PROPERTIES);
		CASE_CL_ERROR(CL_OUT_OF_RESOURCES);
		CASE_CL_ERROR(CL_INVALID_BUFFER_SIZE);
		CASE_CL_ERROR(CL_INVALID_HOST_PTR);
		CASE_CL_ERROR(CL_MEM_OBJECT_ALLOCATION_FAILURE);
		CASE_CL_ERROR(CL_INVALID_DEVICE_TYPE);
		CASE_CL_ERROR(CL_INVALID_COMMAND_QUEUE);
		CASE_CL_ERROR(CL_INVALID_MEM_OBJECT);
		CASE_CL_ERROR(CL_INVALID_EVENT_WAIT_LIST);
		CASE_CL_ERROR(CL_MISALIGNED_SUB_BUFFER_OFFSET);
		CASE_CL_ERROR(CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST);
		default: return "UNKOWN ERROR CODE";
	}//end of switch 
}//end of method 


// The query_platform_info method 
// Purpose: To print the information supplied by the clGetPlatformInfo API 
// Preconditions: cl_platform_id of the platform being queried
// Postconditions: NONE 
void query_platform_info(cl_platform_id platform) {

	cl_int err; 
	size_t param_size; 
	char* param_value; 
	
	int querySize = 5;
	int i;

	const char* infoStr[querySize];
	infoStr[0] = "Profile";
	infoStr[1] = "Version";
	infoStr[2] = "Name";
	infoStr[3] = "Vendor";
	infoStr[4] = "Extensions";

	cl_platform_info param_name[querySize]; 
	param_name[0] = CL_PLATFORM_PROFILE;
	param_name[1] = CL_PLATFORM_VERSION;
	param_name[2] = CL_PLATFORM_NAME;
	param_name[3] = CL_PLATFORM_VENDOR;
	param_name[4] = CL_PLATFORM_EXTENSIONS;

	printf("\n\n\nPlatform Info \n\n");

	for(i = 0; i<querySize; i++){
		err = clGetPlatformInfo(platform,param_name[i],0,NULL,&param_size);
		CHECK_STATUS(err);

		param_value = (char *)malloc(param_size); 

		err = clGetPlatformInfo(platform, param_name[i], param_size, param_value, NULL); 
		CHECK_STATUS(err);

 		printf("%s: %s\n",infoStr[i],param_value);
		
		free(param_value);
	}

}//end of method 


// The query_device_info method
// Purpose:	To display details about the device to stdout. 
// Precondtions:  The cl_device_id returned from a valid API clGetDeviceId call
// Postcondtions: Device info has been displayed to the user via stdout. 
// 				  All allocated memory has been deallocated. 
// note: There are other device informations calls that are not called in this function. 
// see https://www.khronos.org/registry/OpenCL/sdk/1.1/docs/man/xhtml/ for more info. 
void query_device_info(cl_device_id device) {

	cl_int err; 
	size_t param_size;
	char* param_value;

	int querySize = 5;
	int i;

	const char* infoStr[querySize];
	infoStr[0] = "Name";
	infoStr[1] = "Version";
	infoStr[2] = "Driver Version";
	infoStr[3] = "Profile"; 
	infoStr[4] = "Extensions"; 

	cl_device_info param_name[querySize];
	param_name[0] = CL_DEVICE_NAME;
	param_name[1] = CL_DEVICE_VERSION;
	param_name[2] = CL_DRIVER_VERSION;
	param_name[3] = CL_DEVICE_PROFILE;
	param_name[4] = CL_DEVICE_EXTENSIONS;

	printf("\n\nDevice Info \n");

	for (i = 0; i < querySize; i++) {
		err = clGetDeviceInfo(device, param_name[i], 0, NULL, &param_size);
		CHECK_STATUS(err);

		param_value = (char*)malloc(param_size);

		err = clGetDeviceInfo(device, param_name[i], param_size, param_value, NULL);
		CHECK_STATUS(err);

		printf("%s: %s\n",infoStr[i],param_value);
		
		free(param_value);
	}

	// display important memory attributes 
	
	// Global Memory Details  
	printf("\n\nGlobal Memory Cache\n");
	
	cl_ulong cacheSize; 
	err = clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, sizeof(cl_ulong), &cacheSize, NULL);
	CHECK_STATUS(err);
	printf("\n\tSize: %lu\n",(unsigned long)cacheSize);
	
	cl_device_mem_cache_type cacheType;
	err = clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_CACHE_TYPE, sizeof(cl_device_mem_cache_type), &cacheType, NULL);
	CHECK_STATUS(err);

	char* cacheTypeStr; 	
	switch(cacheType) {
		case CL_NONE: cacheTypeStr = "CL_NONE"; break;
		case CL_READ_ONLY_CACHE: cacheTypeStr = "READ ONLY";break;
		case CL_READ_WRITE_CACHE: cacheTypeStr = "READ/WRITE";break;
		default: cacheTypeStr = "no type was found."; 

	}//end of switch 
	printf("\n\tType: %s\n",cacheTypeStr);

	cl_int cacheLineSize;
	err = clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE, sizeof(cl_int), &cacheLineSize, NULL);
	CHECK_STATUS(err);
	printf("\n\tCacheline Size: %d\n",(int)cacheLineSize);

	//constant Memory 
	printf("\n\nConstant Memory\n");

	err = clGetDeviceInfo(device,CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, sizeof(cl_ulong), &cacheSize, NULL);
	CHECK_STATUS(err);
	printf("\n\tMax Constant Buffer Size: %lu\n",cacheSize);

	err = clGetDeviceInfo(device, CL_DEVICE_MAX_CONSTANT_ARGS, sizeof(cl_int), &cacheLineSize, NULL);
	CHECK_STATUS(err);	
	printf("\n\tMaximum number of constant arguments: %d\n",(int)cacheLineSize);

	//local memory 	
	printf("\n\nLocal Memory\n");

	err = clGetDeviceInfo(device, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(cl_ulong), &cacheSize, NULL);
	CHECK_STATUS(err);
	printf("\n\tSize: %lu\n",(unsigned long)cacheSize);

	cl_device_local_mem_type localType;
	err = clGetDeviceInfo(device, CL_DEVICE_LOCAL_MEM_TYPE, sizeof(cl_device_local_mem_type), &localType, NULL);
	CHECK_STATUS(err);

	if(localType == CL_LOCAL) {
		printf("\n\tType: CL_LOCAL\n");
	} else {
		printf("\n\tType: CL_GLOBAL\n");
	}

	// other device informations
	printf("\nOther Device Info\n");

	//work group size 
	err = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &param_size, NULL);
	CHECK_STATUS(err);	
	printf("\n\tMaximum Work-group Size: %d\n",(int)param_size);

	//work Item size 
	size_t* NDrange[3];
	err = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(NDrange), NDrange, NULL);
	CHECK_STATUS(err);	
	printf("\n\tMaximum Work-item Sizes: %d , %d, %d\n",NDrange[0],NDrange[1],NDrange[2]);

	//max work item dimensions 
	err = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(cl_int), &cacheLineSize, NULL);
	CHECK_STATUS(err);
	printf("\n\tMax Work-item Dimensions: %d\n",(int)cacheLineSize);

	err = clGetDeviceInfo(device, CL_DEVICE_ADDRESS_BITS, sizeof(cl_int), &cacheLineSize, NULL);
	CHECK_STATUS(err);
	printf("\n\tAddress Space: %d\n",(int)cacheLineSize);

	err = clGetDeviceInfo(device,CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(cl_ulong), &cacheSize, NULL);
	CHECK_STATUS(err);
	printf("\n\tMax Size of Memory Object Allocation: %lu\n",(unsigned long)cacheSize);

	err = clGetDeviceInfo(device, CL_DEVICE_MAX_PARAMETER_SIZE, sizeof(size_t), &param_size, NULL);
	CHECK_STATUS(err);
	printf("\n\tMaximum Parameter Size: %d\n",(int)param_size);
	
	cl_bool myBool; 
	err = clGetDeviceInfo(device, CL_DEVICE_ENDIAN_LITTLE, sizeof(cl_bool), &myBool, NULL);
	CHECK_STATUS(err);

	if(myBool == CL_TRUE) {
		printf("\n\tLittle Endian\n");
	} else {
		printf("\n\tBig Endian\n");
	}

	err = clGetDeviceInfo(device,CL_DEVICE_IMAGE_SUPPORT, sizeof(cl_bool), &myBool, NULL);
	CHECK_STATUS(err);

	if(myBool == CL_TRUE) {
		printf("\n\tImages Supported\n");
	} else {
		printf("\n\tImages NOT Supported\n");
	}

	printf("\n\n\n");
}//end of method 

