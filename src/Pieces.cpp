
#include "Pieces.h"

Pieces::Pieces(const bool color, const int row, const char column, const bool isSelected) : pieceColor(color), pieceRow(row), pieceColumn(column), pieceSelected(isSelected)
{
    //ctor
}

Pieces::~Pieces()
{
    //dtor
}

void Pieces::setColor(const bool color)
{
    pieceColor=color;
}

void Pieces::setColumn(const char column)
{
    pieceColumn=column;
}


void Pieces::setRow(const int row)
{
    pieceRow=row;
}

void Pieces::setSelectedPiece(const bool isSelected)
{
    pieceSelected=isSelected;
}

bool Pieces::getColor() const
{
    return pieceColor;
}


char Pieces::getColumn() const
{
    return pieceColumn;
}


int Pieces::getRow() const
{
    return pieceRow;
}


char Pieces::getNotation() const
{
    return pieceNotation;
}

bool Pieces::getSelected() const
{
    return pieceSelected;
}

Pieces::PieceType Pieces::getPieceType() const
{
    return pieceType;
}

