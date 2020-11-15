/*#include <iostream>
#include <cstring>
#include <cstdlib>
#include <iomanip>
#include <cassert>

#include <Cl/cl.h>

using namespace std;
			
int main()
{
	const char * required_platform_subname = "Intel";
	cl_int err = CL_SUCCESS;							//CL_SUCCESS表示成功

	cl_uint num_of_platforms = 0;						//遍历系统中所有的OpenCL平台
	err = clGetPlatformIDs(0, 0, &num_of_platforms);

	if (err != CL_SUCCESS)						
	{											
		cerr									\
		<< "OpenCl error with code " << err		\
		<< "happened in file " << __FILE__		\
		<< "at line " << __LINE__				\
		<< ". Exiting...\n";					\
		exit(1);								\
	}
	cout << "Number of available platform: " << num_of_platforms << endl;
	cl_platform_id* platforms = new cl_platform_id[num_of_platforms];
	//得到所有平台ID
	err = clGetPlatformIDs(num_of_platforms, platforms, 0);
	if (err != CL_SUCCESS)						
	{											
		cerr									\
		<< "OpenCl error with code " << err		\
		<< "happened in file " << __FILE__		\
		<< "at line " << __LINE__				\
		<< ". Exiting...\n";					\
		exit(1);								\
	}
	//列出所有平台
	cl_uint selected_platform_index = num_of_platforms;
	cout << "Platform names:\n";
	for (cl_uint i = 0;i < num_of_platforms;++i)
	{
		size_t platform_name_length = 0;
		err = clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, 0, 0, &platform_name_length);//得到名称长度
		if (err != CL_SUCCESS)
		{
			cerr									\
			<< "OpenCl error with code " << err		\
			<< "happened in file " << __FILE__		\
			<< "at line " << __LINE__				\
			<< ". Exiting...\n";					\
			exit(1);								\
		}
		char* platform_name = new char[platform_name_length];
		err = clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, platform_name_length, platform_name, 0);
		if (err != CL_SUCCESS)
		{
			cerr									\
			<< "OpenCl error with code " << err		\
			<< "happened in file " << __FILE__		\
			<< "at line " << __LINE__				\
			<< ". Exiting...\n";					\
			exit(1);								\
		}
		cout << "    [" << i << "] " << platform_name;
		if (strstr(platform_name, required_platform_subname) && selected_platform_index == num_of_platforms)
		{
			cout << " [Selected]";
			selected_platform_index = i;
		}

		cout << endl;
		delete platform_name;
	}
	delete platforms;
	system("pause");
}*/

#include <CL/cl.h>
#include <stdio.h>
#include <iostream>

int main()
{

	unsigned int i, j;									//iterator variables for loops
	cl_int err;
	cl_platform_id platforms[32];						//an array to hold the IDs of all the platforms, hopefuly there won't be more than 32
	cl_uint num_platforms;								//this number will hold the number of platforms on this machine
	char vendor[1024];									//this strirng will hold a platforms vendor
	cl_device_id devices[16];							//this variable holds the number of devices for each platform, hopefully it won't be more than 32 per platform
														//32 devices doesn't work with TDM-GCC 4.9.2 64bit
	cl_uint num_devices;								//this number will hold the number of devices on this machine
	char deviceName[1024];								//this string will hold the devices name
	cl_uint numberOfCores;								//this variable holds the number of cores of on a device
	cl_long amountOfMemory;								//this variable holds the amount of memory on a device
	cl_uint clockFreq;									//this variable holds the clock frequency of a device
	cl_ulong maxAlocatableMem;							//this variable holds the maximum allocatable memory
	cl_ulong localMem;									//this variable holds local memory for a device
	cl_bool  available;									//this variable holds if the device is available

	char buildinkernels[1024];
	//get the number of platforms
	clGetPlatformIDs(32, platforms, &num_platforms);
	printf("\nNumber of platforms:\t%u\n\n", num_platforms);
	//this is a loop for platforms
	for (i = 0; i < num_platforms; i++)
	{
		printf("Platform:\t\t%u\n\n", i);
		err = clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, sizeof(vendor), vendor, NULL);
		printf("\tPlatform Vendor:\t%s (err==%d,%d)\n", vendor, err, CL_SUCCESS);
		err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, sizeof(devices), devices, &num_devices);
		printf("\tNumber of devices:\t%u  (err==%d,%d) \n\n", num_devices, err, CL_SUCCESS);
		//this is a loop for devices
		for (j = 0; j < num_devices; j++)
		{
			//scan in device information
			clGetDeviceInfo(devices[j], CL_DEVICE_NAME, sizeof(deviceName), deviceName, NULL);
			clGetDeviceInfo(devices[j], CL_DEVICE_VENDOR, sizeof(vendor), vendor, NULL);
			clGetDeviceInfo(devices[j], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(numberOfCores), &numberOfCores, NULL);
			clGetDeviceInfo(devices[j], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(amountOfMemory), &amountOfMemory, NULL);
			clGetDeviceInfo(devices[j], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(clockFreq), &clockFreq, NULL);
			clGetDeviceInfo(devices[j], CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(maxAlocatableMem), &maxAlocatableMem, NULL);
			clGetDeviceInfo(devices[j], CL_DEVICE_LOCAL_MEM_SIZE, sizeof(localMem), &localMem, NULL);
			clGetDeviceInfo(devices[j], CL_DEVICE_AVAILABLE, sizeof(available), &available, NULL);
			clGetDeviceInfo(devices[j], CL_DEVICE_BUILT_IN_KERNELS, sizeof(buildinkernels), &buildinkernels, NULL);
			//print out device information
			printf("\tDevice: %u\n", j);
			printf("\t\tName:\t\t\t\t%s\n", deviceName);
			printf("\t\tVendor:\t\t\t\t%s\n", vendor);
			printf("\t\tAvailable:\t\t\t%s\n", available ? "Yes" : "No");
			printf("\t\tCompute Units:\t\t\t%u\n", numberOfCores);
			printf("\t\tClock Frequency:\t\t%u MHz\n", clockFreq);
			printf("\t\tGlobal Memory:\t\t\t%0.00f MB\n", (double)amountOfMemory / 1048576);
			printf("\t\tMax Allocateable Memory:\t%0.00f MB\n", (double)maxAlocatableMem / 1048576);
			printf("\t\tLocal Memory:\t\t\t%u KB\n", (unsigned int)localMem);
			printf("\t\tBuild In Kernels:\t\t%s\n", buildinkernels);
			clGetDeviceInfo(devices[j], CL_DEVICE_OPENCL_C_VERSION, sizeof(buildinkernels), &buildinkernels, NULL);
			printf("\t\tOpenCL Version:\t\t\t%s\n", buildinkernels);
			clGetDeviceInfo(devices[j], CL_DEVICE_PROFILE, sizeof(buildinkernels), &buildinkernels, NULL);
			printf("\t\tDevice profile:\t\t\t%s\n", buildinkernels);
			clGetDeviceInfo(devices[j], CL_DEVICE_VERSION, sizeof(buildinkernels), &buildinkernels, NULL);
			printf("\t\tDevice version:\t\t\t%s\n", buildinkernels);
			clGetDeviceInfo(devices[j], CL_DRIVER_VERSION, sizeof(buildinkernels), &buildinkernels, NULL);
			printf("\t\tDriver version:\t\t\t%s\n", buildinkernels);
			printf("\n");
		}
	}
	system("PAUSE");
	return 0;
}
