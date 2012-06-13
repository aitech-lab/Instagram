#pragma once
class FractalTriangle
{
public:
	FractalTriangle(float x1, float y1, float x2, float y2, float x3, float y3, int l=0);
	~FractalTriangle(void);

	void split(void);

	float x1 , x2 , x3 , y1 , y2 , y3 ;
	float x12, x23, x31, y12, y23, y31;
	FractalTriangle *t1, *t2, *t3, *t4;

	bool hasIntegral;
	unsigned int integral;

	int l;
};

