/*
 * Matrix44.h
 *
 *  Created on: Sep 6, 2013
 *      Author: malvir
 */

#ifndef MATRIX44_H_
#define MATRIX44_H_

#include <math.h>
#include "Vector3.h"

class Matrix44
{
private:

public:
	float matData[4][4];
	Matrix44(){
	}

	void setIdentity()
	{
		for(int i=0;i<4;i++)
		{
			for(int j=0;j<4;j++)
				this->matData[i][j]=0;
		}
		this->matData[0][0] = 1;
		this->matData[1][1] = 1;
		this->matData[2][2] = 1;
		this->matData[3][3] = 1;
		return;
	}

	void setScale(float x);
	void setTranslate(float x, float y, float z);
	void setAsPerspective(float fov, float near, float far, float w , float h);
	void multiply(Vector3 *v);

	Matrix44 operator*(const Matrix44 &b);
	void operator*=(const Matrix44 &b);
	~Matrix44(){
	}
};



#endif /* MATRIX44_H_ */
