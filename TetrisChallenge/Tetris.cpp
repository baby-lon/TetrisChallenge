#include <windows.h>
#include "Tetris.h"
#include <ctime>

////////////////////////////
//[ 0][ 0]→[ 0][ 9]
// ↓
// ↓落ちる方向は+になる
// ↓
//[19][ 0]→[19][ 9]
//
//

//////////////////
// 位置
//////////////////

CPosition& CPosition::operator+=(const CPosition& obj){
	X += obj.X;
	Y += obj.Y;
	return *this;
}


//////////////////////////////
// ブロック

CBlock::CBlock():
Color(COLOR_EMPTY)
{
}

CBlock::~CBlock(){
}

CTetrimino::CTetrimino():
MaxRotate(1),
RotateCount(0),
MaxPiece(0)
{
}


CTetrimino::~CTetrimino(){
}

int CTetrimino::MoveBlock(){
	return false;
}

int CTetrimino::Rotate(int count){
	RotateCount+=count;
	if(RotateCount<0){
		RotateCount += MaxRotate;
	}
	RotateCount %= MaxRotate;
	return false;
}

CPosition CTetrimino::GetPiece(int index)const{
	CPosition rot_piece(Piece[index]);
	for(int ii=0; ii<RotateCount; ii++){
		//90度の回転行列をかけている
		int x = rot_piece.GetY();
		int y = rot_piece.GetX()*-1;
		rot_piece.SetX(x);
		rot_piece.SetY(y);
	}
	return rot_piece;
}



///////////////////
//
//
//
//
//////////////////
CNullTeti::CNullTeti()
{
	MaxPiece = 0;
	Piece = NULL;
}

CNullTeti::~CNullTeti(){
	CTetrimino::~CTetrimino();
}

///////////////////
//
//■■■■
//
//
//////////////////
CITetri::CITetri()
{
	Color = COLOR_RED;
	MaxRotate = 2;
	MaxPiece = 4;
	Piece = new CPosition[4];

	Piece[0] = CPosition(-2, 0);
	Piece[1] = CPosition(-1, 0);
	Piece[2] = CPosition(1, 0);
	Piece[3] = CPosition(0, 0);
}

CITetri::~CITetri(){
	delete [] Piece;
	CTetrimino::~CTetrimino();
}

///////////////////
//
//■■
//■■
//
//////////////////
COTetri::COTetri()
{
	Color = COLOR_YELLOW;
	MaxRotate = 1;
	MaxPiece = 4;
	Piece = new CPosition[4];

	Piece[0] = CPosition(1, 0);
	Piece[1] = CPosition(0, 1);
	Piece[2] = CPosition(1, 1);
	Piece[3] = CPosition(0, 0);
}


COTetri::~COTetri(){
	delete [] Piece;
	CTetrimino::~CTetrimino();
}

///////////////////
//
//　■■
//■■
//
//////////////////
CSTetri::CSTetri()
{
	Color = COLOR_POEPLE;
	MaxRotate = 2;
	MaxPiece = 4;
	Piece = new CPosition[4];

	Piece[0] = CPosition(1, 0);
	Piece[1] = CPosition(-1, 1);
	Piece[2] = CPosition(0, 1);
	Piece[3] = CPosition(0, 0);
}

CSTetri::~CSTetri(){
	delete [] Piece;
	CTetrimino::~CTetrimino();
}

///////////////////
//
//■■
//　■■
//
//////////////////
CZTetri::CZTetri()
{
	Color = COLOR_GREEN;
	MaxRotate = 2;
	MaxPiece = 4;
	Piece = new CPosition[4];

	Piece[0] = CPosition(-1, 0);
	Piece[1] = CPosition(0, 1);
	Piece[2] = CPosition(1, 1);
	Piece[3] = CPosition(0, 0);
}

CZTetri::~CZTetri(){
	delete [] Piece;
	CTetrimino::~CTetrimino();
}

///////////////////
//
//■■■
//　　■
//
//////////////////
CJTetri::CJTetri()
{
	Color = COLOR_BLUE;
	MaxRotate = 4;
	MaxPiece = 4;
	Piece = new CPosition[4];

	Piece[0] = CPosition(-1, 0);
	Piece[1] = CPosition(1, 0);
	Piece[2] = CPosition(1, 1);
	Piece[3] = CPosition(0, 0);
}

CJTetri::~CJTetri(){
	delete [] Piece;
	CTetrimino::~CTetrimino();
}


///////////////////
//
//■■■
//■
//
//////////////////
CLTetri::CLTetri()
{
	Color = COLOR_ORANGE;
	MaxRotate = 4;
	MaxPiece = 4;
	Piece = new CPosition[4];

	Piece[0] = CPosition(-1, 0);
	Piece[1] = CPosition(1, 0);
	Piece[2] = CPosition(-1, 1);
	Piece[3] = CPosition(0, 0);
}

CLTetri::~CLTetri(){
	delete [] Piece;
	CTetrimino::~CTetrimino();
}


///////////////////
//
//■■■
//　■
//
//////////////////
CTTetri::CTTetri()
{
	Color = COLOR_CYAN;
	MaxRotate = 4;
	MaxPiece = 4;
	Piece = new CPosition[4];

	Piece[0] = CPosition(-1, 0);
	Piece[1] = CPosition(1, 0);
	Piece[2] = CPosition(0, 1);
	Piece[3] = CPosition(0, 0);
}


CTTetri::~CTTetri(){
	delete [] Piece;
	CTetrimino::~CTetrimino();
}


///////////////////
//
//　テトリス本体
//
///////////////////
CTetris::CTetris():
BlockImage(NULL),
hInstance(NULL),
BufferDC(NULL),
MoveTetri(NULL),
IsComplete(NULL),
GameState(SEQUENCETYPE_NULL),
IntervalFrame(10),
Frame(-1),
IsSpeedUp(false)
{
	CBlock empty, wall;
	empty.SetColor(COLOR_EMPTY);
	wall.SetColor(COLOR_WALL);
	srand(static_cast<unsigned>(time(NULL)));

	for(int y=0; y<BOARD_HEIGHT; y++){
		for(int x=0; x<BOARD_WIDTH; x++){
			Board[x][y]=empty;
			if(x==0 || x==BOARD_WIDTH-1 || y==0 || y==BOARD_HEIGHT-1){
				Board[x][y] = wall;
			}
		}
	}
	GameState = SEQUENCETYPE_GAME;

}

CTetris::~CTetris(){
	if(BlockImage != NULL){
		DeleteObject(BlockImage);
	}
	if(BlockDC != NULL){
		DeleteDC(BlockDC);
	}
	if(BufferImage != NULL){
		DeleteObject(BufferImage);
	}
	if(BufferDC != NULL){
		DeleteDC(BufferDC);
	}

	if(MoveTetri != NULL){
		delete MoveTetri;
		MoveTetri = NULL;
	}
	if(IsComplete != NULL){
		delete [] IsComplete;
	}
}

bool CTetris::CreateBuffer(HINSTANCE instance, HWND hwnd){
	HDC dc = GetDC(hwnd);
	BlockDC = CreateCompatibleDC(dc);
	BlockImage = LoadBitmap(instance, TEXT("BLOCKS"));
	SelectObject(BlockDC, BlockImage);
	BufferDC = CreateCompatibleDC(dc);
	BufferImage = CreateCompatibleBitmap(dc, GetBitmapWidth(), GetBitmapHeight());
	SelectObject(BufferDC, BufferImage);
	ReleaseDC(hwnd, dc);

	return true;
}


void CTetris::Move(){
	Frame++;
	if(GameState == SEQUENCETYPE_GAMEOVER){
		return;
	}
	if(Frame % IntervalFrame != 0 && IsSpeedUp == false){
		return;
	}
	if(MoveTetri == NULL){
		MoveTetri = CreateTetri();
		CPosition default_pos(BoardWidth/2 -1, 0);
		MoveTetri->SetPos(default_pos);
	}
	else{
		CPosition check_pos(MoveTetri->GetPos());
		check_pos += CPosition(0, 1);
		int ret = CheckMove(check_pos);
		if(ret == 0){
			//移動できる
			MoveTetri->SetPos(check_pos);
		}
		else{
			//移動できない。位置固定。
			CopyToBorad(*MoveTetri);
			delete MoveTetri;
			MoveTetri = NULL;
		}
	}
	//列チェック
	IsComplete = new bool[BoardHeight];
	int complete_line = 0;
	for(int y=1; y<BoardHeight-1; y++){
		IsComplete[y] = true;
		for(int x=1; x<BoardWidth-1; x++){
			if(Board[x][y].GetColor() == COLOR_EMPTY){
				IsComplete[y] = false;
			}
		}
		if(IsComplete[y]==true){
			for(int x=1; x<BoardWidth-1; x++){
				Board[x][y].SetColor(COLOR_EMPTY);
			}
			complete_line++;
		}
	}

	//列を詰める
	if(0<complete_line){
		for(int x=0; x<BoardWidth; x++){
			for(int y=BoardHeight-2, dest_y = BoardHeight-2; 1<dest_y; y--){
				if(IsComplete[y] == false){
					if(0<y){
						Board[x][dest_y].SetColor(Board[x][y].GetColor());
					}
					else{
						Board[x][dest_y].SetColor(COLOR_EMPTY);
					}
					dest_y--;
				}
			}
		}
	}

	delete [] IsComplete;
	IsComplete = NULL;

	//ゲームオーバー判定
	for(int x=1; x<BoardWidth-1; x++){
		if(Board[x][1].GetColor() != COLOR_EMPTY){
			GameState = SEQUENCETYPE_GAMEOVER;
		}
	}
}


void CTetris::Draw(){
	for(int y=0; y<BoardHeight; y++){
		for(int x=0; x<BoardWidth; x++){
			int img_pos = BlockSize * Board[x][y].GetColor();
			::BitBlt(
				BufferDC, BlockSize*x, BlockSize*y, 
				BlockSize, BlockSize, 
				BlockDC, 0, img_pos, SRCCOPY
				);
		}
	}

	//テトリミノの描画
	if(MoveTetri != NULL){
		for(int ii=0; ii<MoveTetri->GetMaxPiece(); ii++){
			CPosition pos = MoveTetri->GetPos();
			pos += MoveTetri->GetPiece(ii);
			int x_pos = BlockSize * pos.GetX();
			int y_pos = BlockSize * pos.GetY();
			int img_pos = BlockSize * MoveTetri->GetColor();
			::BitBlt(
				BufferDC, x_pos, y_pos, 
				BlockSize, BlockSize, 
				BlockDC, 0, img_pos, SRCCOPY
				);
		}
	}
	//がめおべら
	if(GameState == SEQUENCETYPE_GAMEOVER){
		RECT rect;
		rect.top = 100;
		rect.bottom = 150;
		rect.left = 0;
		rect.right = GetBitmapWidth();
		DrawText(BufferDC, TEXT("GAME OVER"), lstrlen(TEXT("GAME OVER")), &rect, DT_CENTER | DT_VCENTER);
	}
}

void CTetris::BitBlt(HDC mem_dc){
	::BitBlt(mem_dc, 0, 0, GetBitmapWidth(), GetBitmapHeight(), BufferDC, 0, 0, SRCCOPY);
}


void CTetris::Drop(){
}

CTetrimino* CTetris::CreateTetri(){
	CTetrimino* tetrimino = NULL;
	int type = rand() % 7;
	switch(type){
		case 0:
			tetrimino = new CITetri();
			break;
		case 1:
			tetrimino = new COTetri();
			break;
		case 2:
			tetrimino = new CSTetri();
			break;
		case 3:
			tetrimino = new CZTetri();
			break;
		case 4:
			tetrimino = new CJTetri();
			break;
		case 5:
			tetrimino = new CLTetri();
			break;
		case 6:
			tetrimino = new CTTetri();
			break;
	}

	return tetrimino;
}

//移動できる？
//引数:x-次に移動する位置のxインデックス,y-次に移動する位置のxインデックス
//戻り値:移動できたら0、出来なかったら0以外
int CTetris::CheckMove(CPosition& pos, int rotate) const{
	int ret = 0;
	MoveTetri->Rotate(rotate);
	for(int ii=0; ii<MoveTetri->GetMaxPiece(); ii++){
		CPosition check_pos(pos);
		check_pos += MoveTetri->GetPiece(ii);
		CBlock piece = Board[check_pos.GetX()][check_pos.GetY()];
		if(piece.GetColor() != COLOR_EMPTY){
			ret++;
		}
	}
	MoveTetri->Rotate(rotate*-1);
	return ret;
}

int CTetris::CopyToBorad(CTetrimino& tetri){
	for(int ii=0; ii<tetri.GetMaxPiece(); ii++){
		CPosition pos(tetri.GetPos());
		pos += tetri.GetPiece(ii);
		Board[pos.GetX()][pos.GetY()].SetColor(tetri.GetColor());
	}
	return 0;
}

int CTetris::MovePiece(int direction){
	if(MoveTetri == NULL){
		return 0;//移動するものがない
	}
	CPosition amount;
	if(direction<0){
		//左へ移動
		amount = CPosition(-1, 0);
	}
	if(0<direction){
		//右へ移動
		amount = CPosition(1, 0);
	}
	CPosition pos(MoveTetri->GetPos());
	pos += amount;
	int ret = CheckMove(pos);
	if(ret == 0){
		//移動する
		MoveTetri->SetPos(pos);
	}
	
	return 0;
}

int CTetris::RotatePiece(){
	if(MoveTetri == NULL){
		return 1;
	}
	CPosition pos(MoveTetri->GetPos());
	int ret = CheckMove(pos, 1);
	if(ret == 0){
		MoveTetri->Rotate();
	}
	return ret;
}

