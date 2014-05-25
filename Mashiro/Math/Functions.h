#ifndef INCLUDE_MASHIRO_MATH_FUNCTIONS_H_
#define INCLUDE_MASHIRO_MATH_FUNCTIONS_H_

#undef min
#undef max

//数学ライブラリー。角度単位は度で返します
namespace Mashiro {

namespace Math{

//三角関数
float sin( float );
float cos( float );
float tan( float );
//逆三角関数
float asin( float );
float acos( float );
float atan( float );
float atan2( float, float );

float pow( float a, float b );//べき乗(aのb乗)
float log2( float );//2を低とする対数
float log10( float );//10を低とする対数
float abs( float );//絶対値
float min( float, float );//最少
float max( float, float );//最大
float sqrt( float );//平方根

const float EPSILON = 00.00000011920928955078125f;
const float FLOAT_MAX = 1.7014117331926442990585209174226e+38;

bool isSpecial( float );//NaN,Infならtrue

}
}

#endif
