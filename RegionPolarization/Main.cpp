#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>

// default length unit = A
#define MAX_LINE_LENGTH_POW 15 // this means MAX_LINE_LENGTH = 1 << 15
#define MAX_NAME_LENGTH 1000
#define MAX_DIFF_ELEMENTS 100
#define MAX_TOTAL_ELEMENTS 1000
#define MAX_ELEMENT_NAME 5
#define MAX_GRID 100

struct vector3D {
	double x, y, z;
	vector3D() : x(0.0), y(0.0), z(0.0) {}
	vector3D(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
	double Norm() {
		return std::sqrt(x * x + y * y + z * z);
	};
	vector3D Base() {
		double Norm = this->Norm();
		return vector3D(x / Norm, y / Norm, z / Norm);
	}
};

struct structure {
	char Name[MAX_NAME_LENGTH];
	double Scale;
	vector3D a, b, c;
	int DiffElements;
	char ElementName[MAX_DIFF_ELEMENTS][MAX_ELEMENT_NAME];
	int ElementCnt[MAX_DIFF_ELEMENTS];
	char CoordinateName[MAX_NAME_LENGTH];
	int TotalElements;
	vector3D ElementPos[MAX_TOTAL_ELEMENTS];
	int NGXF, NGYF, NGZF;
	double Data[MAX_GRID][MAX_GRID][MAX_GRID]; 
	void Import();
	void Print();
} Structure;

int main() {
	CHGCAR = fopen("CHGCAR", "r");
	Structure.Import();
	fclose(CHGCAR);
	return 0;
}

char Buffer[1 << MAX_LINE_LENGTH_POW], *BufferL, *BufferR;
FILE* CHGCAR;

inline int GetChar() {
	if (BufferL == BufferR) 
		if (BufferL == (BufferR = (BufferL = Buffer) + fread(Buffer, 1, 1 << MAX_LINE_LENGTH_POW, CHGCAR)))
			return -1;
	return (int)(*BufferL++);
}

inline void GetLine(char* St) {
	memset(St, 0, sizeof(St));
	int Len = 0;
	for (int Ch = GetChar(); Ch != (int)'\n'; Ch = GetChar())
		St[Len++] = Ch;
	return;
}

inline void GetString(char* St) {
	memset(St, 0, sizeof(St));
	int Len = 0;
	int Ch = GetChar();
	while ((Ch < (int)'a' || Ch > (int)'z') &&
			(Ch < (int)'A' || Ch > (int)'Z'))
		Ch = GetChar();
	while ((Ch >= (int)'a' && Ch <= (int)'z') ||
			(Ch >= (int)'A' && Ch <= (int)'Z')) {
		St[Len++] = Ch;
		Ch = GetChar();
	}
	return;
}

inline int GetInt() {
	int Ans = 0;
	int Ch = GetChar();
	int Flg = 1;
	while (Ch < (int)'0' || Ch (int)'9') {
		if (Ch == '-') Flg = -1;
		Ch = GetChar();
	}
	while (Ch >= (int)'0' && Ch <= (int)'9') {
		Ans = Ans * 10 + (Ch - (int)'0');
		Ch = GetChar();
	}
	return Ans * Flg;
}

inline double GetFloat() {
	double Ans = 0;
	int Ch = GetChar();
	while (Ch < (int)'0' || Ch > (int)'9') Ch = GetChar();
	Ans = Ans * 10 + (Ch - (int)'0');
	if (Ch == (int)'.') {
		Ch = GetChar();
		int k = 0.1;
		while (Ch >= (int)'0' && Ch <= (int)'9') {
			Ans += k * (Ch - (int)'0');
			k *= 0.1;
			Ch = GetChar();
		}
	}
	if (Ch == (int)'E' || Ch == (int)'e') {
		Ans *= std::pow(10.0, GetInt());
	}
	return Ans;
}

void structure::Import() {
	GetLine(Name);
	Scale = GetFloat();
	a.x = GetFloat(); a.y = GetFloat(); a.z = GetFloat();
	b.x = GetFloat(); b.y = GetFloat(); b.z = GetFloat();
	c.x = GetFloat(); c.y = GetFloat(); c.z = GetFloat();

	DiffElements = 0;
	memset(ElementName, 0, sizeof(ElementName));
	int Ch = GetChar();
	while ((Ch < (int)'a' || Ch > (int)'z') && 
			(Ch < (int)'A' || Ch > (int)'Z')) Ch = GetChar();
	for (;;) {
		int Len = 0;
		ElementName[DiffElements][Len++] = Ch;
		while ((Ch >= (int)'a' && Ch <= (int) 'z') || 
				(Ch >= (int)'A' && Ch <= (int) 'Z')) {
			ElementName[DiffElements][Len++] = Ch;
			Ch = GetChar();
		}
		++DiffElements;
		while ((Ch < (int)'a' || Ch > (int)'z') &&
				(Ch < (int)'A' || Ch > (int) 'Z')) {
			if (Ch == (int)'\n') break;
			Ch = GetChar();
		}
		if (Ch == (int)'\n') break;
	}

	for (int i = 0; i < DiffElements; ++i) 
		ElementCnt[i] = GetInt();
	GetString(CoordinateName);
	
	TotalElements = 0;
	for (int i = 0; i < DiffElements; ++i) 
		for (int j = 0; j < ElementCnt[i]; ++j) {
			ElementPos[TotalElements].x = GetFloat();
			ElementPos[TotalElements].y = GetFloat();
			ElementPos[TotalElements].z = GetFloat();
			++TotalElements;
		}

	NGXF = GetInt(); NGYF = GetInt(); NGZF = GetInt();

	for (int NZ = 0; NZ < NGZF; ++NZ)
		for (int NY = 0; NY < NGYF; ++NY)
			for (int NX = 0; NX < NGXF; ++NX)
				Data[NX][NY][NZ] = GetFloat();
	return;
}

void structure::Print() {
	printf("%s\n", Name);
	printf("%.10lf\n", Scale);
	printf("%.10lf    %.10lf    %.10lf\n", &a.x &a.y, &a.z);
	printf("%.10lf    %.10lf    %.10lf\n", &b.x &b.y, &b.z);
	printf("%.10lf    %.10lf    %.10lf\n", &c.x &c.y, &c.z);
	return;
}
