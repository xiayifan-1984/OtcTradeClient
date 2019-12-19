#include "updownlineedit.h"

UpDownLineEdit::UpDownLineEdit(QWidget* parent /* = nullptr*/)
    :QLineEdit(parent)
{

}

UpDownLineEdit::~UpDownLineEdit()
{

}

void UpDownLineEdit::keyPressEvent(QKeyEvent *e)
{
     int key = e->key();
      if (Qt::Key_Down == key)
      {
          emit editdownkey();
      }
      else if(Qt::Key_Up == key)
      {
          emit editupkey();
      }

      QLineEdit::keyPressEvent(e);
}
