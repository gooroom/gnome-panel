#include <gio/gio.h>

#if defined (__ELF__) && ( __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 6))
# define SECTION __attribute__ ((section (".gresource.fish"), aligned (8)))
#else
# define SECTION
#endif

static const SECTION union { const guint8 data[2680]; const double alignment; void * const ptr;}  fish_resource_data = { {
  0x47, 0x56, 0x61, 0x72, 0x69, 0x61, 0x6e, 0x74, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x18, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x28, 0x08, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 
  0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 
  0x03, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 
  0x07, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 
  0xb0, 0xb7, 0x24, 0x30, 0x02, 0x00, 0x00, 0x00, 
  0x00, 0x01, 0x00, 0x00, 0x06, 0x00, 0x4c, 0x00, 
  0x08, 0x01, 0x00, 0x00, 0x0c, 0x01, 0x00, 0x00, 
  0x93, 0x1c, 0x9c, 0x02, 0x06, 0x00, 0x00, 0x00, 
  0x0c, 0x01, 0x00, 0x00, 0x07, 0x00, 0x76, 0x00, 
  0x18, 0x01, 0x00, 0x00, 0x27, 0x09, 0x00, 0x00, 
  0x4b, 0x50, 0x90, 0x0b, 0x03, 0x00, 0x00, 0x00, 
  0x27, 0x09, 0x00, 0x00, 0x04, 0x00, 0x4c, 0x00, 
  0x2c, 0x09, 0x00, 0x00, 0x30, 0x09, 0x00, 0x00, 
  0xd4, 0xb5, 0x02, 0x00, 0xff, 0xff, 0xff, 0xff, 
  0x30, 0x09, 0x00, 0x00, 0x01, 0x00, 0x4c, 0x00, 
  0x34, 0x09, 0x00, 0x00, 0x38, 0x09, 0x00, 0x00, 
  0x44, 0x9f, 0x99, 0xd8, 0x07, 0x00, 0x00, 0x00, 
  0x38, 0x09, 0x00, 0x00, 0x07, 0x00, 0x4c, 0x00, 
  0x40, 0x09, 0x00, 0x00, 0x44, 0x09, 0x00, 0x00, 
  0xf5, 0x15, 0x13, 0x56, 0x06, 0x00, 0x00, 0x00, 
  0x44, 0x09, 0x00, 0x00, 0x0c, 0x00, 0x76, 0x00, 
  0x50, 0x09, 0x00, 0x00, 0x5f, 0x0a, 0x00, 0x00, 
  0xbd, 0x17, 0x85, 0xaa, 0x04, 0x00, 0x00, 0x00, 
  0x5f, 0x0a, 0x00, 0x00, 0x05, 0x00, 0x4c, 0x00, 
  0x64, 0x0a, 0x00, 0x00, 0x6c, 0x0a, 0x00, 0x00, 
  0x6f, 0xd8, 0xf6, 0x94, 0x00, 0x00, 0x00, 0x00, 
  0x6c, 0x0a, 0x00, 0x00, 0x06, 0x00, 0x4c, 0x00, 
  0x74, 0x0a, 0x00, 0x00, 0x78, 0x0a, 0x00, 0x00, 
  0x67, 0x6e, 0x6f, 0x6d, 0x65, 0x2f, 0x00, 0x00, 
  0x07, 0x00, 0x00, 0x00, 0x66, 0x69, 0x73, 0x68, 
  0x2e, 0x75, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x9b, 0x50, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 
  0x78, 0xda, 0xed, 0x5c, 0x4b, 0x73, 0xdb, 0x36, 
  0x10, 0xbe, 0xe7, 0x57, 0xa0, 0xb8, 0x76, 0x64, 
  0x99, 0x72, 0x1d, 0x3b, 0x19, 0x89, 0x99, 0xd4, 
  0x6d, 0xd2, 0x43, 0xa7, 0x93, 0x69, 0xdd, 0x33, 
  0x07, 0x22, 0x57, 0x12, 0x22, 0x08, 0x60, 0x01, 
  0xd0, 0x8f, 0x5f, 0x91, 0x63, 0xff, 0x5f, 0x7f, 
  0x49, 0x41, 0xd2, 0x0f, 0xca, 0xa2, 0xf8, 0x00, 
  0x28, 0x5b, 0x72, 0xe8, 0x9b, 0xa9, 0xdd, 0x25, 
  0xf6, 0xc3, 0xbe, 0x80, 0x05, 0x31, 0xfe, 0x70, 
  0xb3, 0x62, 0xe8, 0x0a, 0xa4, 0xa2, 0x82, 0x4f, 
  0xb0, 0x77, 0x74, 0x8c, 0x11, 0xf0, 0x50, 0x44, 
  0x94, 0xcf, 0x27, 0xf8, 0xef, 0xcb, 0x4f, 0x83, 
  0x73, 0xfc, 0xc1, 0x7f, 0x33, 0xfe, 0x61, 0x30, 
  0x40, 0x9f, 0x81, 0x83, 0x24, 0x1a, 0x22, 0x74, 
  0x4d, 0xf5, 0x02, 0xcd, 0x19, 0x89, 0x00, 0x9d, 
  0x1c, 0x79, 0xe7, 0x47, 0x27, 0x68, 0x30, 0x30, 
  0x44, 0x94, 0x6b, 0x90, 0x33, 0x12, 0x82, 0xff, 
  0x06, 0xa1, 0xb1, 0x84, 0x7f, 0x12, 0x2a, 0x41, 
  0x21, 0x46, 0xa7, 0x13, 0x3c, 0xd7, 0xcb, 0x1f, 
  0xf1, 0xe3, 0x8b, 0x0c, 0xdb, 0x08, 0x0f, 0x33, 
  0x3a, 0x31, 0xfd, 0x0a, 0xa1, 0x46, 0x21, 0x23, 
  0x4a, 0x4d, 0xf0, 0x67, 0xbd, 0xfc, 0x18, 0x7d, 
  0x4d, 0x94, 0x5e, 0x01, 0xd7, 0x18, 0xd1, 0x68, 
  0x82, 0xc9, 0xc3, 0xff, 0x1e, 0x4e, 0x39, 0x0c, 
  0x4f, 0x2c, 0x45, 0x0c, 0x52, 0xdf, 0x22, 0x4e, 
  0x56, 0x30, 0xc1, 0x4c, 0x5c, 0x83, 0xc4, 0xbe, 
  0x37, 0x1e, 0xde, 0xff, 0x50, 0x4e, 0x97, 0xc4, 
  0x71, 0x4a, 0x37, 0x3a, 0x3d, 0xad, 0xa3, 0xbc, 
  0x22, 0x2c, 0x01, 0xec, 0x9f, 0xd4, 0xd1, 0x29, 
  0x0d, 0x71, 0x40, 0x79, 0x28, 0x21, 0x1b, 0x6f, 
  0xfd, 0x10, 0x62, 0x32, 0x87, 0x22, 0xc3, 0x93, 
  0x91, 0x8c, 0x87, 0x39, 0x1c, 0x6d, 0x91, 0x19, 
  0x55, 0x23, 0x73, 0x7c, 0xe4, 0x1d, 0xaf, 0xff, 
  0x35, 0xc5, 0xca, 0x3b, 0x6e, 0x08, 0x95, 0xd7, 
  0x16, 0x2a, 0x8b, 0x31, 0x3d, 0x05, 0xcf, 0x6b, 
  0x03, 0xde, 0x2f, 0x94, 0x30, 0x31, 0xcf, 0x81, 
  0x9b, 0x51, 0xb5, 0x08, 0x62, 0x09, 0x33, 0x90, 
  0xc6, 0xda, 0x41, 0x05, 0x51, 0xfe, 0x63, 0xf9, 
  0x6b, 0x43, 0xc2, 0x83, 0x99, 0x08, 0x13, 0x85, 
  0xfd, 0x4f, 0x84, 0x29, 0xa8, 0x1b, 0xe6, 0x54, 
  0xc8, 0x08, 0x64, 0x70, 0x4d, 0x23, 0xbd, 0xd8, 
  0x98, 0xe1, 0x4d, 0x72, 0x4d, 0x35, 0x03, 0x8c, 
  0xb4, 0x24, 0x5c, 0x31, 0xa2, 0xc9, 0x94, 0x99, 
  0x87, 0xb7, 0x90, 0xbe, 0xcd, 0x0c, 0x13, 0x7d, 
  0x79, 0x1c, 0x66, 0xad, 0xa4, 0xdb, 0x18, 0x82, 
  0x05, 0x4d, 0xa1, 0xc9, 0x15, 0xda, 0x60, 0x08, 
  0x17, 0x94, 0x45, 0x28, 0x73, 0x54, 0x4e, 0xd8, 
  0x20, 0xfb, 0xd7, 0x4c, 0xe1, 0x54, 0xdc, 0xdc, 
  0xe9, 0x5e, 0x86, 0xdc, 0xcf, 0xe6, 0xd7, 0x0c, 
  0xb6, 0x5c, 0xe8, 0x20, 0x25, 0x1f, 0x3d, 0xd0, 
  0x97, 0x58, 0x04, 0x55, 0xd4, 0x28, 0x81, 0xfd, 
  0x4b, 0x99, 0x6c, 0x80, 0x65, 0x03, 0x70, 0x19, 
  0x8f, 0x90, 0xd4, 0xd8, 0x00, 0xd1, 0x26, 0x9c, 
  0x60, 0xdf, 0xc4, 0x15, 0x4d, 0x43, 0xc2, 0x9a, 
  0x30, 0xaa, 0x98, 0x84, 0x26, 0xba, 0x99, 0x30, 
  0x50, 0x4a, 0x5d, 0x8e, 0x10, 0x09, 0xd3, 0x17, 
  0x05, 0x44, 0x02, 0x29, 0x28, 0x5e, 0x0a, 0x56, 
  0xa2, 0xb5, 0xe0, 0x4f, 0x21, 0x2b, 0xf0, 0x8f, 
  0xd6, 0x04, 0x58, 0xa0, 0x67, 0x8b, 0xe0, 0xa3, 
  0x7a, 0xeb, 0xcf, 0xb6, 0x6b, 0x91, 0xab, 0xb0, 
  0x00, 0x16, 0x07, 0xd3, 0xfc, 0xc1, 0x53, 0xce, 
  0x92, 0x88, 0x43, 0xa6, 0xc0, 0x4a, 0xad, 0x39, 
  0xf8, 0xcd, 0x08, 0xda, 0x36, 0x30, 0x6b, 0x24, 
  0x6a, 0xd0, 0xb0, 0x61, 0x8d, 0x60, 0x46, 0x12, 
  0xa6, 0xdb, 0x33, 0x4b, 0x08, 0x81, 0x5e, 0xa5, 
  0xe1, 0xe4, 0x5e, 0x42, 0xe5, 0x64, 0x94, 0x86, 
  0x5d, 0x05, 0x41, 0xc2, 0x4d, 0xf8, 0x60, 0x94, 
  0xd7, 0x6a, 0x5e, 0x0c, 0x78, 0xeb, 0x42, 0x49, 
  0xb8, 0x34, 0x36, 0x5e, 0xff, 0x3a, 0xb8, 0x89, 
  0x09, 0x8f, 0x2c, 0xc6, 0x39, 0xa3, 0x8c, 0x59, 
  0xb0, 0xc5, 0x42, 0xd1, 0xdc, 0x65, 0x8f, 0xab, 
  0xd4, 0x2a, 0x1d, 0xff, 0x78, 0x58, 0x62, 0xbc, 
  0x36, 0x06, 0x1d, 0x09, 0x0e, 0x9d, 0x18, 0xf4, 
  0x05, 0x13, 0xed, 0xd4, 0x3f, 0x38, 0x8b, 0x5e, 
  0x10, 0xd5, 0xbb, 0xc3, 0x73, 0xb8, 0x83, 0xd7, 
  0x85, 0x3b, 0x94, 0x01, 0x50, 0xae, 0xbc, 0x95, 
  0xe2, 0x16, 0x4a, 0x6f, 0x56, 0x70, 0xe1, 0x32, 
  0x48, 0xcb, 0x14, 0xec, 0x03, 0x8f, 0x1a, 0x73, 
  0xd5, 0x45, 0x8d, 0x12, 0x88, 0x36, 0xe0, 0xd9, 
  0x8c, 0x14, 0x15, 0x95, 0x4e, 0x5a, 0xe2, 0x9c, 
  0xbe, 0x68, 0xa2, 0x6e, 0x57, 0x53, 0x3a, 0x57, 
  0x49, 0x95, 0x95, 0x92, 0x77, 0xee, 0x5c, 0x4f, 
  0x14, 0x81, 0x7d, 0x8b, 0x9f, 0x27, 0x52, 0xb6, 
  0x76, 0xc7, 0x56, 0x68, 0x55, 0x22, 0xf6, 0xb6, 
  0x92, 0xab, 0x14, 0xb4, 0x72, 0xe0, 0x7e, 0xcf, 
  0xb3, 0x4f, 0x0a, 0x5d, 0x96, 0x88, 0xbc, 0x13, 
  0x5c, 0xc6, 0xe8, 0x00, 0xa0, 0x2b, 0x88, 0x65, 
  0xfc, 0x37, 0x84, 0xd1, 0x79, 0x75, 0x92, 0x6f, 
  0x9b, 0x6c, 0xf3, 0xdd, 0x07, 0x56, 0x2b, 0x90, 
  0x68, 0x2d, 0xa9, 0x49, 0xee, 0xa0, 0xca, 0x09, 
  0x8a, 0x24, 0x77, 0x2f, 0xbd, 0x06, 0x3a, 0x5f, 
  0x98, 0x25, 0x75, 0xb6, 0x92, 0x4d, 0x5d, 0x8d, 
  0x45, 0xf9, 0xde, 0x44, 0x09, 0xef, 0xb0, 0x5a, 
  0xfe, 0xb6, 0x14, 0x54, 0x9d, 0x86, 0x9c, 0x52, 
  0x91, 0x43, 0x3a, 0xb2, 0xae, 0xd0, 0x2a, 0xd2, 
  0xd2, 0xd6, 0x4a, 0xad, 0xad, 0xe5, 0x3f, 0x84, 
  0x8c, 0xc5, 0x96, 0x90, 0xf1, 0xf2, 0x56, 0xbf, 
  0x55, 0x9b, 0x86, 0xbe, 0xbc, 0x4d, 0x2b, 0x67, 
  0xcd, 0xba, 0xd0, 0x6e, 0xbb, 0x7b, 0xfa, 0x77, 
  0xd3, 0x5f, 0xc3, 0x5e, 0xe5, 0x0b, 0xf5, 0xfe, 
  0xe0, 0xec, 0x13, 0x8e, 0x7e, 0xe1, 0xe4, 0x1b, 
  0x35, 0xfe, 0x51, 0xe9, 0x23, 0x36, 0x96, 0xb5, 
  0x96, 0x5e, 0xcf, 0x0e, 0xcd, 0xaa, 0x5a, 0xa7, 
  0x5d, 0xb7, 0xf4, 0xdb, 0x0c, 0xe4, 0x1a, 0xa0, 
  0xd3, 0x17, 0x06, 0xc5, 0x6d, 0xb3, 0x1d, 0xe1, 
  0xdd, 0x15, 0xe6, 0xd5, 0xf5, 0xdd, 0xa8, 0xb1, 
  0x88, 0x5a, 0xcc, 0x6a, 0x43, 0x5f, 0x86, 0xdc, 
  0x5d, 0x20, 0xa9, 0x91, 0xd3, 0x11, 0x7e, 0x5d, 
  0x62, 0xe8, 0x54, 0xed, 0x58, 0x6e, 0x33, 0xfc, 
  0x61, 0x08, 0x90, 0x98, 0xa1, 0x74, 0xd7, 0xfa, 
  0xbd, 0xcb, 0x3b, 0xd2, 0xd5, 0xf4, 0x8a, 0xc8, 
  0x65, 0x12, 0xbb, 0x03, 0xd8, 0x6a, 0x65, 0x5e, 
  0x2f, 0x6e, 0xc5, 0x61, 0x25, 0x38, 0x0d, 0xd3, 
  0x15, 0xce, 0x1c, 0x34, 0xf6, 0x33, 0x33, 0x31, 
  0x61, 0x41, 0xde, 0x36, 0x17, 0x5b, 0x97, 0x71, 
  0x9a, 0x67, 0x9e, 0xce, 0x32, 0x50, 0x47, 0x99, 
  0xa8, 0x93, 0x8c, 0xd4, 0x30, 0x33, 0x35, 0xca, 
  0x50, 0xae, 0x01, 0xe1, 0xd7, 0x74, 0x66, 0x0b, 
  0x01, 0x21, 0x9b, 0xe9, 0x3d, 0x09, 0x08, 0xae, 
  0xa2, 0xd2, 0xcd, 0xfe, 0x2b, 0xa2, 0xa1, 0xc5, 
  0xd6, 0xd9, 0x4b, 0xd9, 0xb1, 0xab, 0xaa, 0xb9, 
  0x19, 0xbb, 0x4a, 0x69, 0xb4, 0x0d, 0xb6, 0x3b, 
  0x2b, 0x6e, 0x02, 0x77, 0x33, 0xa8, 0xbb, 0x80, 
  0xd9, 0x1d, 0x62, 0xe7, 0x20, 0xd1, 0x00, 0xda, 
  0x5a, 0x58, 0x9d, 0xaa, 0xab, 0x50, 0xac, 0x56, 
  0x06, 0xb8, 0xbe, 0xc0, 0xb2, 0x29, 0xb0, 0xee, 
  0xc1, 0xeb, 0x6b, 0xac, 0xe6, 0x35, 0xd6, 0x85, 
  0x08, 0x32, 0xd0, 0x90, 0x16, 0x48, 0x26, 0x1c, 
  0x5d, 0x2f, 0x80, 0x1b, 0x74, 0x69, 0xb8, 0x84, 
  0xe8, 0xfd, 0x3e, 0x97, 0x49, 0xf7, 0x93, 0xdd, 
  0x57, 0x4a, 0xdf, 0x41, 0xa5, 0xb4, 0x36, 0xd9, 
  0x7d, 0xb1, 0xd4, 0x17, 0x4b, 0x7d, 0xb1, 0xb4, 
  0xc3, 0x62, 0xc9, 0x7b, 0xae, 0x62, 0x69, 0x77, 
  0x1b, 0xa5, 0xb6, 0xbb, 0x7a, 0xed, 0x60, 0x74, 
  0x82, 0xd0, 0x7e, 0x9b, 0xb4, 0xfb, 0x5e, 0x8b, 
  0x4d, 0xd3, 0xa0, 0x39, 0x54, 0xd6, 0x30, 0xb5, 
  0xef, 0xb4, 0x74, 0x77, 0x12, 0xa2, 0x6d, 0xfb, 
  0xb5, 0x19, 0x1c, 0xfb, 0x73, 0x2c, 0xa8, 0xb8, 
  0x71, 0x7e, 0xde, 0xf7, 0xa5, 0xdb, 0x77, 0xe7, 
  0x0c, 0x6e, 0xde, 0x21, 0xb4, 0xa4, 0x1f, 0xd7, 
  0x62, 0xde, 0xce, 0xfb, 0x7a, 0xa3, 0x43, 0xeb, 
  0xc0, 0xb4, 0x5a, 0x5d, 0x35, 0x5f, 0x55, 0x7d, 
  0xe4, 0x74, 0x95, 0x59, 0x6d, 0xa3, 0x25, 0x54, 
  0x83, 0xd6, 0xbd, 0x6b, 0xfb, 0xbe, 0x49, 0x0b, 
  0xbf, 0x6f, 0x5d, 0x3e, 0x6b, 0xeb, 0xf2, 0x42, 
  0xac, 0xa6, 0xc2, 0x44, 0x93, 0x4b, 0xb8, 0xb9, 
  0xfb, 0xaa, 0xc1, 0xd8, 0xcc, 0x1c, 0x82, 0x30, 
  0x7d, 0x6e, 0x82, 0x8b, 0x4e, 0x9f, 0xef, 0xb5, 
  0x3b, 0xf5, 0xe5, 0xdb, 0x9e, 0x94, 0x6f, 0x0e, 
  0x47, 0x65, 0xdc, 0xea, 0xb7, 0x7d, 0x3a, 0x29, 
  0x73, 0xf6, 0x2a, 0x4f, 0xca, 0x9c, 0x1d, 0xe6, 
  0x49, 0x99, 0xb2, 0xa4, 0xd8, 0x1f, 0x9e, 0xd9, 
  0xe7, 0xc3, 0x33, 0xef, 0xfa, 0xc3, 0x33, 0xbb, 
  0x3f, 0x3c, 0xa3, 0x62, 0x80, 0xbe, 0xb9, 0x63, 
  0xd5, 0xdc, 0xc9, 0xa1, 0xeb, 0x5b, 0x3b, 0x2d, 
  0x8e, 0xcf, 0x7c, 0x21, 0x89, 0x02, 0x64, 0x88, 
  0xd1, 0x4c, 0x1a, 0xda, 0xbd, 0x6e, 0xe7, 0xe4, 
  0xd3, 0xab, 0x62, 0xca, 0xfb, 0x5e, 0xce, 0x2b, 
  0xec, 0xe5, 0xfc, 0x65, 0x26, 0xb6, 0xf8, 0x25, 
  0xda, 0xe3, 0x74, 0xbf, 0x92, 0x6e, 0x0e, 0xe5, 
  0x77, 0xa3, 0x0a, 0xc2, 0x05, 0x91, 0xd8, 0xff, 
  0xef, 0xdf, 0x6f, 0x4e, 0xcd, 0xa1, 0xc7, 0x0f, 
  0xde, 0xfd, 0xc2, 0xc7, 0xee, 0x4e, 0xca, 0x32, 
  0xba, 0x9a, 0x06, 0xe9, 0x3d, 0x06, 0xc5, 0x4f, 
  0xd0, 0xbd, 0x9f, 0xde, 0xb9, 0x08, 0x8d, 0xe8, 
  0x9c, 0x6a, 0xd5, 0xaa, 0x23, 0xb3, 0x29, 0x84, 
  0x27, 0x2b, 0x90, 0x34, 0xdc, 0xdf, 0xfe, 0xd7, 
  0xfe, 0xb9, 0xbf, 0x77, 0x58, 0xee, 0xbf, 0x91, 
  0xc7, 0x8d, 0x26, 0xba, 0x4f, 0xe6, 0x2d, 0x92, 
  0xb9, 0x82, 0x50, 0xf0, 0x48, 0xf5, 0xdd, 0xe1, 
  0x7a, 0xdf, 0x18, 0xbd, 0xce, 0xee, 0xb0, 0x53, 
  0xcd, 0x6e, 0x11, 0x80, 0xf6, 0xa0, 0x3f, 0x6c, 
  0xb7, 0xda, 0xba, 0x58, 0x40, 0xb8, 0xf6, 0xd9, 
  0xbb, 0x14, 0x66, 0x99, 0x08, 0x81, 0x16, 0xf3, 
  0x79, 0x1a, 0x35, 0xde, 0x74, 0xe3, 0x91, 0xc1, 
  0x9f, 0x99, 0x58, 0x24, 0x38, 0xba, 0x5f, 0x7d, 
  0x22, 0x33, 0x53, 0xc0, 0x94, 0x2d, 0xc2, 0x5d, 
  0xaf, 0xf2, 0x5c, 0xc4, 0x58, 0x7c, 0xc2, 0xbe, 
  0xa3, 0xe5, 0xc3, 0xd6, 0x40, 0x7b, 0x74, 0x6a, 
  0x2b, 0x22, 0x92, 0xe4, 0x3a, 0xa0, 0x3c, 0x32, 
  0x73, 0xa6, 0x85, 0x6c, 0x33, 0x9c, 0xef, 0xc9, 
  0xcd, 0x4f, 0xfa, 0x63, 0x20, 0xfd, 0x31, 0x90, 
  0xfe, 0x18, 0xc8, 0xf3, 0x1e, 0x03, 0x39, 0xdc, 
  0xeb, 0x30, 0xda, 0x5c, 0x06, 0x51, 0xab, 0x6f, 
  0xfd, 0xbd, 0x16, 0xeb, 0x3a, 0xae, 0xfd, 0x38, 
  0xce, 0x2f, 0x9d, 0x1a, 0xe4, 0x7b, 0x4b, 0x0f, 
  0xbd, 0xee, 0xf5, 0xc7, 0x48, 0x82, 0x8a, 0x05, 
  0x57, 0x66, 0x34, 0x03, 0xcf, 0xc3, 0x7e, 0xe1, 
  0xb2, 0xa7, 0xf1, 0x70, 0x8d, 0xb2, 0x9e, 0xff, 
  0x14, 0xfb, 0x85, 0xab, 0x75, 0x4a, 0xd9, 0x9f, 
  0x3c, 0x54, 0xeb, 0xd7, 0xb6, 0x8d, 0x87, 0x85, 
  0x2b, 0x04, 0xff, 0x07, 0x27, 0xc4, 0x12, 0xe9, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x28, 0x75, 0x75, 0x61, 0x79, 0x29, 0x6f, 
  0x72, 0x67, 0x2f, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x2f, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 
  0x61, 0x70, 0x70, 0x6c, 0x65, 0x74, 0x2f, 0x00, 
  0x06, 0x00, 0x00, 0x00, 0x66, 0x69, 0x73, 0x68, 
  0x2d, 0x6d, 0x65, 0x6e, 0x75, 0x2e, 0x75, 0x69, 
  0xf6, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 
  0x78, 0xda, 0x75, 0x4f, 0xcb, 0x0e, 0xc3, 0x20, 
  0x0c, 0x3b, 0x8f, 0xaf, 0x40, 0xb9, 0x6f, 0xfd, 
  0x01, 0xca, 0x37, 0xec, 0x17, 0x02, 0x73, 0x35, 
  0x24, 0x60, 0x13, 0xa4, 0x87, 0xfd, 0xfd, 0x78, 
  0x48, 0xed, 0xa9, 0x37, 0xdb, 0x89, 0x13, 0xdb, 
  0x84, 0x2c, 0x28, 0x1b, 0x7b, 0x58, 0xa5, 0xb5, 
  0x49, 0xc8, 0xbb, 0x0e, 0xaf, 0x95, 0xb6, 0x50, 
  0xdf, 0xf7, 0xce, 0xa8, 0xeb, 0x6d, 0x52, 0xe1, 
  0x25, 0x7c, 0xb2, 0x55, 0xb7, 0x41, 0x83, 0x20, 
  0x35, 0x3c, 0x09, 0x8b, 0x94, 0xe0, 0x76, 0x81, 
  0xce, 0x9c, 0xb0, 0x52, 0x64, 0x87, 0x48, 0x5a, 
  0x0a, 0xe7, 0x1a, 0x59, 0xd8, 0xc5, 0x26, 0xfe, 
  0x50, 0xc9, 0x3e, 0x0b, 0x36, 0x14, 0x64, 0x8f, 
  0x6a, 0x96, 0xc3, 0x76, 0x79, 0x88, 0xc7, 0x4f, 
  0xb2, 0x3d, 0xcd, 0xe3, 0x7b, 0xe5, 0x1d, 0xd6, 
  0x65, 0x26, 0x9a, 0xf8, 0x08, 0xdb, 0x49, 0x6f, 
  0x61, 0x55, 0x5b, 0x38, 0x9b, 0xfe, 0x01, 0x97, 
  0x76, 0x4e, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x28, 0x75, 0x75, 0x61, 0x79, 0x29, 0x66, 
  0x69, 0x73, 0x68, 0x2f, 0x05, 0x00, 0x00, 0x00, 
  0x01, 0x00, 0x00, 0x00, 0x70, 0x61, 0x6e, 0x65, 
  0x6c, 0x2f, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00
} };

static GStaticResource static_resource = { fish_resource_data.data, sizeof (fish_resource_data.data), NULL, NULL, NULL };
extern GResource *fish_get_resource (void);
GResource *fish_get_resource (void)
{
  return g_static_resource_get_resource (&static_resource);
}
/*
  If G_HAS_CONSTRUCTORS is true then the compiler support *both* constructors and
  destructors, in a sane way, including e.g. on library unload. If not you're on
  your own.

  Some compilers need #pragma to handle this, which does not work with macros,
  so the way you need to use this is (for constructors):

  #ifdef G_DEFINE_CONSTRUCTOR_NEEDS_PRAGMA
  #pragma G_DEFINE_CONSTRUCTOR_PRAGMA_ARGS(my_constructor)
  #endif
  G_DEFINE_CONSTRUCTOR(my_constructor)
  static void my_constructor(void) {
   ...
  }

*/

#ifndef __GTK_DOC_IGNORE__

#if  __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 7)

#define G_HAS_CONSTRUCTORS 1

#define G_DEFINE_CONSTRUCTOR(_func) static void __attribute__((constructor)) _func (void);
#define G_DEFINE_DESTRUCTOR(_func) static void __attribute__((destructor)) _func (void);

#elif defined (_MSC_VER) && (_MSC_VER >= 1500)
/* Visual studio 2008 and later has _Pragma */

#define G_HAS_CONSTRUCTORS 1

/* We do some weird things to avoid the constructors being optimized
 * away on VS2015 if WholeProgramOptimization is enabled. First we
 * make a reference to the array from the wrapper to make sure its
 * references. Then we use a pragma to make sure the wrapper function
 * symbol is always included at the link stage. Also, the symbols
 * need to be extern (but not dllexport), even though they are not
 * really used from another object file.
 */

/* We need to account for differences between the mangling of symbols
 * for Win32 (x86) and x64 programs, as symbols on Win32 are prefixed
 * with an underscore but symbols on x64 are not.
 */
#ifdef _WIN64
#define G_MSVC_SYMBOL_PREFIX ""
#else
#define G_MSVC_SYMBOL_PREFIX "_"
#endif

#define G_DEFINE_CONSTRUCTOR(_func) G_MSVC_CTOR (_func, G_MSVC_SYMBOL_PREFIX)
#define G_DEFINE_DESTRUCTOR(_func) G_MSVC_DTOR (_func, G_MSVC_SYMBOL_PREFIX)

#define G_MSVC_CTOR(_func,_sym_prefix) \
  static void _func(void); \
  extern int (* _array ## _func)(void);              \
  int _func ## _wrapper(void) { _func(); g_slist_find (NULL,  _array ## _func); return 0; } \
  __pragma(comment(linker,"/include:" _sym_prefix # _func "_wrapper")) \
  __pragma(section(".CRT$XCU",read)) \
  __declspec(allocate(".CRT$XCU")) int (* _array ## _func)(void) = _func ## _wrapper;

#define G_MSVC_DTOR(_func,_sym_prefix) \
  static void _func(void); \
  extern int (* _array ## _func)(void);              \
  int _func ## _constructor(void) { atexit (_func); g_slist_find (NULL,  _array ## _func); return 0; } \
   __pragma(comment(linker,"/include:" _sym_prefix # _func "_constructor")) \
  __pragma(section(".CRT$XCU",read)) \
  __declspec(allocate(".CRT$XCU")) int (* _array ## _func)(void) = _func ## _constructor;

#elif defined (_MSC_VER)

#define G_HAS_CONSTRUCTORS 1

/* Pre Visual studio 2008 must use #pragma section */
#define G_DEFINE_CONSTRUCTOR_NEEDS_PRAGMA 1
#define G_DEFINE_DESTRUCTOR_NEEDS_PRAGMA 1

#define G_DEFINE_CONSTRUCTOR_PRAGMA_ARGS(_func) \
  section(".CRT$XCU",read)
#define G_DEFINE_CONSTRUCTOR(_func) \
  static void _func(void); \
  static int _func ## _wrapper(void) { _func(); return 0; } \
  __declspec(allocate(".CRT$XCU")) static int (*p)(void) = _func ## _wrapper;

#define G_DEFINE_DESTRUCTOR_PRAGMA_ARGS(_func) \
  section(".CRT$XCU",read)
#define G_DEFINE_DESTRUCTOR(_func) \
  static void _func(void); \
  static int _func ## _constructor(void) { atexit (_func); return 0; } \
  __declspec(allocate(".CRT$XCU")) static int (* _array ## _func)(void) = _func ## _constructor;

#elif defined(__SUNPRO_C)

/* This is not tested, but i believe it should work, based on:
 * http://opensource.apple.com/source/OpenSSL098/OpenSSL098-35/src/fips/fips_premain.c
 */

#define G_HAS_CONSTRUCTORS 1

#define G_DEFINE_CONSTRUCTOR_NEEDS_PRAGMA 1
#define G_DEFINE_DESTRUCTOR_NEEDS_PRAGMA 1

#define G_DEFINE_CONSTRUCTOR_PRAGMA_ARGS(_func) \
  init(_func)
#define G_DEFINE_CONSTRUCTOR(_func) \
  static void _func(void);

#define G_DEFINE_DESTRUCTOR_PRAGMA_ARGS(_func) \
  fini(_func)
#define G_DEFINE_DESTRUCTOR(_func) \
  static void _func(void);

#else

/* constructors not supported for this compiler */

#endif

#endif /* __GTK_DOC_IGNORE__ */

#ifdef G_HAS_CONSTRUCTORS

#ifdef G_DEFINE_CONSTRUCTOR_NEEDS_PRAGMA
#pragma G_DEFINE_CONSTRUCTOR_PRAGMA_ARGS(resource_constructor)
#endif
G_DEFINE_CONSTRUCTOR(resource_constructor)
#ifdef G_DEFINE_DESTRUCTOR_NEEDS_PRAGMA
#pragma G_DEFINE_DESTRUCTOR_PRAGMA_ARGS(resource_destructor)
#endif
G_DEFINE_DESTRUCTOR(resource_destructor)

#else
#warning "Constructor not supported on this compiler, linking in resources will not work"
#endif

static void resource_constructor (void)
{
  g_static_resource_init (&static_resource);
}

static void resource_destructor (void)
{
  g_static_resource_fini (&static_resource);
}
