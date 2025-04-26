// Badger
#pragma once

#include <string>
#include <cstring>
#include <cassert>
#include <cstdint>

using namespace std;

template<typename T=uint8_t>
struct rgb{
	T r,g,b;

	rgb(T r=0, T g=0, T b=0):r(r),g(g),b(b){}
	template<typename A>rgb(const rgb<A>& c):r(c.r),g(c.g),b(c.b){}
	rgb operator+(const rgb& c) const{
		return rgb(r+c.r,g+c.g,b+c.b);
	}
	rgb operator-(const rgb& c) const{
		return rgb(r-c.r,g-c.g,b-c.b);
	}
	rgb operator*(const rgb& c) const{
		return rgb(r*c.r,g*c.g,b*c.b);
	}
	rgb operator/(const rgb& c) const{
		return rgb(r/c.r,g/c.g,b/c.b);
	}
	rgb operator*(const float& f) const{
		return rgb(r*f,g*f,b*f);
	}
	rgb operator/(const float& f) const{
		return rgb(r/f,g/f,b/f);
	}
	rgb operator+=(const rgb& c){
		return rgb(r+=c.r,g+=c.g,b+=c.b);
	}
	rgb operator-=(const rgb& c){
		return rgb(r-=c.r,g-=c.g,b-=c.b);
	}
	rgb operator*=(const rgb& c){
		return rgb(r*=c.r,g*=c.g,b*=c.b);
	}
	rgb operator/=(const rgb& c){
		return rgb(r/=c.r,g/=c.g,b/=c.b);
	}
	rgb operator*=(const float& f){
		return rgb(r*=f,g*=f,b*=f);
	}
	rgb operator/=(const float& f){
		return rgb(r/=f,g/=f,b/=f);
	}
	rgb operator=(const rgb& c){
		return rgb(r=c.r,g=c.g,b=c.b);
	}
	rgb operator=(const uint8_t& c){
		return rgb(r=c,g=c,b=c);
	}
	rgb operator=(const string& s){
		assert(s.size()==7);
		assert(s[0]=='#');
		r=stoi(s.substr(1,2),nullptr,16);
		g=stoi(s.substr(3,2),nullptr,16);
		b=stoi(s.substr(5,2),nullptr,16);
		return *this;
	}
	bool operator==(const rgb& c) const{
		return r==c.r&&g==c.g&&b==c.b;
	}
	bool operator!=(const rgb& c) const{
		return !(*this==c);
	}
	bool operator<(const rgb& c) const{
		return r<c.r&&g<c.g&&b<c.b;
	}
	bool operator>(const rgb& c) const{
		return r>c.r&&g>c.g&&b>c.b;
	}
	bool operator<=(const rgb& c) const{
		return r<=c.r&&g<=c.g&&b<=c.b;
	}
	bool operator>=(const rgb& c) const{
		return r>=c.r&&g>=c.g&&b>=c.b;
	}
	bool operator==(const uint8_t& c) const{
		return r==c&&g==c&&b==c;
	}
	bool operator!=(const uint8_t& c) const{
		return !(*this==c);
	}
	bool operator<(const uint8_t& c) const{
		return r<c&&g<c&&b<c;
	}
	bool operator>(const uint8_t& c) const{
		return r>c&&g>c&&b>c;
	}
	bool operator<=(const uint8_t& c) const{
		return r<=c&&g<=c&&b<=c;
	}
	bool operator>=(const uint8_t& c) const{
		return r>=c&&g>=c&&b>=c;
	}
	operator rgb<float>() const{
		return rgb<float>((float)r/255,(float)g/255,(float)b/255);
	}
	operator string() const {
		return "\033[38;2;"+to_string(r)+";"+to_string(g)+";"+to_string(b)+"m";
	}
};

using rgb8 = rgb<uint8_t>;

rgb8 abs(const rgb8& c){
	return rgb8(abs(c.r),abs(c.g),abs(c.b));
}

const rgb8 black(0x00,0x00,0x00);
const rgb8 white(0xff,0xff,0xff);
const rgb8 red(0xff,0x00,0x00);
const rgb8 orange(0xff,0xa5,0x00);
const rgb8 yellow(0xff,0xff,0x00);
const rgb8 green(0x00,0xff,0x00);
const rgb8 blue(0x00,0x00,0xff);
const rgb8 indigo(0x4b,0x00,0x82);
const rgb8 violet(0xee,0x82,0xee);