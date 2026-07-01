The NV21 format is not played successfully, that is because the kernel does not support NV21 in UVC host driver.
Modify as follows:

1. drivers\media\usb\uvc\uvcvideo.h
add a macro before #define UVC_GUID_FORMAT_NV12 : 
#define UVC_GUID_FORMAT_NV21 \
	{ 'N', 'V', '2', '1', 0x00, 0x00, 0x10, 0x00, \
	0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}

2. drivers\media\usb\uvc\uvc_driver.c
add an struct item in array :
static struct uvc_format_desc uvc_fmts[] ... {
...
    {
        .name = "YUV 4:2:0 (NV21)",
        .guid = UVC_GUID_FORMAT_NV21,
        .fcc = V4L2_PIX_FMT_NV21,
    },
...
}
	
rebuild kernel and burn it.

