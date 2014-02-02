#ifndef INCLUDED_TETRIS_H
#define INCLUDED_TETRIS_H

#include <Windows.h>

class CTetris;

enum ColorType{
	COLOR_EMPTY,
	COLOR_WALL,
	COLOR_GREEN,
	COLOR_CYAN,
	COLOR_YELLOW,
	COLOR_POEPLE,
	COLOR_RED,
	COLOR_ORANGE,
	COLOR_BLUE
};

class CPosition{
public:
	CPosition():X(0),Y(0){}
	CPosition(int x, int y):X(x),Y(y){}
	CPosition(const CPosition& obj):X(obj.GetX()),Y(obj.GetY()){}
	~CPosition(){}
	void SetPos(int x, int y){X=x; Y=y;}
	void SetX(int x){X = x;}
	void SetY(int y){Y = y;}
	const int GetX()const{return X;}
	const int GetY()const{return Y;}
	CPosition& operator+=(const CPosition& obj);

private:
	int X;
	int Y;
};

class CBlock{
public:
	CBlock();
	~CBlock();

	void SetColor(ColorType color){Color = color;}
	ColorType GetColor(){return Color;}

private:
	ColorType Color;
};

class CTetrimino{
public:
	CTetrimino();
	virtual ~CTetrimino();

	int MoveBlock();
	int Rotate(int count=1);
	const CPosition& GetPos()const {return Pos;}
	void SetPos(CPosition& pos){Pos = pos;}
	CPosition GetPiece(int index)const;
	const ColorType GetColor()const{return Color;}
	int GetMaxPiece(){return MaxPiece;}

protected:
	CPosition* Piece;
	int MaxPiece;
	ColorType Color;
	int MaxRotate;
	int RotateCount;
	CPosition Pos;
};

class CNullTeti: public CTetrimino{
public:
	CNullTeti();
	~CNullTeti();
};

class CITetri: public CTetrimino{
public:
	CITetri();
	~CITetri();
};

class COTetri: public CTetrimino{
public:
	COTetri();
	~COTetri();
};

class CSTetri: public CTetrimino{
public:
	CSTetri();
	~CSTetri();
};

class CZTetri: public CTetrimino{
public:
	CZTetri();
	~CZTetri();
};

class CJTetri: public CTetrimino{
public:
	CJTetri();
	~CJTetri();
};

class CLTetri: public CTetrimino{
public:
	CLTetri();
	~CLTetri();
};

class CTTetri: public CTetrimino{
public:
	CTTetri();
	~CTTetri();
};

enum SequenceType{
	SEQUENCETYPE_NULL,
	SEQUENCETYPE_GAME,
	SEQUENCETYPE_GAMEOVER
};

#define BOARD_WIDTH 12
#define BOARD_HEIGHT 22
class CTetris{
public:
	CTetris();
	~CTetris();

	bool CreateBuffer(HINSTANCE instance, HWND hwnd);

	static const int BoardWidth = BOARD_WIDTH;
	static const int BoardHeight = BOARD_HEIGHT;
	static const int BlockSize = 24;

	void Move();
	void Draw();
	void BitBlt(HDC mem_dc);
	int GetBitmapWidth()const{return (BoardWidth) * BlockSize;}
	int GetBitmapHeight()const{return BoardHeight * BlockSize;}
	int CheckMove(CPosition& pos, int rotate=0) const;
	int CopyToBorad(CTetrimino& tetri);
	int MovePiece(int direction);
	int RotatePiece();
	void SpeedUp(bool is_up){IsSpeedUp = is_up;}

	void Drop();
private:
	CBlock Board[BOARD_WIDTH][BOARD_HEIGHT];
	HINSTANCE hInstance;
	HBITMAP BlockImage;
	HDC BlockDC;
	HBITMAP BufferImage;
	HDC BufferDC;

	CTetrimino* MoveTetri;
	bool* IsComplete;
	SequenceType GameState;
	int IntervalFrame;
	int Frame;
	bool IsSpeedUp;

	CTetrimino* CreateTetri();
	
};

#endif
