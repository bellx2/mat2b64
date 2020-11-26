/*
	mat2b64 - cv::Mat to Base64 String

	Ryu Tanabe (https://github.com/bellx2)

*/

#include <iostream>
#include <fstream>
#include <string>
#include <opencv2/opencv.hpp>

#include "cmdline.h"

std::string base64_encode(uchar const* bytes_to_encode, unsigned int in_len) 
{

	static const std::string base64_chars =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz"
		"0123456789+/";

	std::string ret;

	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (in_len--) 
	{
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3)
		{
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (i = 0; (i <4); i++) 
			{
				ret += base64_chars[char_array_4[i]];
			}
			i = 0;
		}
	}

	if (i) 
	{
		for (j = i; j < 3; j++) 
		{
			char_array_3[j] = '\0';
		}

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; (j < i + 1); j++) 
		{
			ret += base64_chars[char_array_4[j]];
		}
		
		while ((i++ < 3)) 
		{
			ret += '=';
		}
	}
	return ret;
}

std::string mat2str(const cv::Mat& m)
{
	std::vector<uchar> buf;
	bool code = cv::imencode(".jpg", m, buf);
	uchar* result = reinterpret_cast<uchar*> (&buf[0]);
	return base64_encode(result, buf.size());
}

int main(int argc, char* argv[]) {

	cmdline::parser p;

	p.add<std::string>("in", 'i', "image file name", true, "");
	p.add<std::string>("out", 'o', "output file name (jpeg && Base64)", false, "");
	p.add("help", 0, "print help");

	if (!p.parse(argc, argv)||p.exist("help")){
	    std::cout<<p.error_full()<<p.usage();
    	return 0;
  	}

	cv::Mat src_img = cv::imread(p.get<std::string>("in"),0);
	std::string b64str = mat2str(src_img);
	if ( p.get<std::string>("out") != "" ){
		std::ofstream of(p.get<std::string>("out"));
		of << b64str << std::endl;
		of.close();
	} else {
		std::cout << "data:image/jpeg;base64," << b64str << std::endl;
	}
	exit(0);
}
