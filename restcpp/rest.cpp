#include <iostream>
#include <stdio.h>
#include <string>
#include "restclient-cpp/restclient.h"
#include <fstream>
#include <cstring> // for std::strlen
#include <cstddef> // for std::size_t -> is a typedef on an unsinged int

#define _BASE64_H_

using namespace std;

static const std::string base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

char* readFileBytes(const char *name);
std::string base64_encode(char const* , unsigned int len);

static inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

int main() {

    std::string idPark      ("9");
    std::string idDrone     ("4");
    std::string carsCount   ("123");
    std::string capturePost = "http://192.168.160.174:8080/api/capture/park/"+ idPark +"/drone/"+ idDrone ;
    
    std::string bodyCapture ("{ \"Park_idPark\" : "+ idPark +", \"Drone_idDrone\" : "+ idDrone +", \"startedTime\" : \"2015-12-02\", \"finishTime\" : \"\", \"nOccupiedLots\" : "+ carsCount +", \"valid\" : \"\" }");
    
    //POST capture
    RestClient::Response capture = RestClient::post( capturePost, "application/json", bodyCapture );
    // get idcapture
    std::string idCapture = capture.body.substr(13, capture.body.find(",") - 13);
    //cout << idCapture << "\n";
    
    // Read Photo
    char* oData = readFileBytes("dbDrone.png");
    std::string encodeData = base64_encode( oData, strlen(oData) );                                                                                                                                                                           
    //print data
	//for ( size_t i = 0; i < strlen(oData); i++ )
	//{
	//	cout << "oData["<<i<<"] " << oData[i];
	//	cout << "\n";
	//}
    
    // POST photo
    std::string bodyPhoto ("{ \"fileName\" : \"teste\", \"latitude\" : \"\", \"longitude\" : \"\", \"heigth\" : \"\", \"photo\" : \""+ encodeData +"\"}");
    cout << bodyPhoto << "\n";
    std::string photoPost = "http://192.168.160.174:8080/api/photo/capture/"+ idCapture ;
    RestClient::Response photo = RestClient::post( photoPost, "application/json", bodyPhoto );
    cout << photo.body << "\n";

}

char* readFileBytes(const char *name)  
{  
    ifstream fl(name);  
    fl.seekg( 0, ios::end );  
    size_t len = fl.tellg();  
    char *ret = new char[len];  
    fl.seekg(0, ios::beg);   
    fl.read(ret, len);  
    fl.close();  
    return ret;  
}  

std::string base64_encode(char const* bytes_to_encode, unsigned int in_len) {
  std::string ret;
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  while (in_len--) {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for(i = 0; (i <4) ; i++)
        ret += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i)
  {
    for(j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++)
      ret += base64_chars[char_array_4[j]];

    while((i++ < 3))
      ret += '=';
  }

  return ret;

}