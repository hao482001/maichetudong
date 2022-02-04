#include<SoftwareSerial.h>
int rxPin = 7;  // cai dat chan truyen du lieu tu module 
int txPin = 6;  // cai dat chan nhan du lieu tu module bluetooth
SoftwareSerial hc06(rxPin, txPin);  // cai dat cac chan truyen nhan du lieu cho module bluetooth

// cai dat hai chan truyen du lieu tu arduino cho l298n
int in1 = 9;
int in2 = 10;

const int buttonCounterPin = 13;   // La chan nhan tin hieu cua nut bam thay doi che do (TU DONG/ THU CONG)
int htMo = 5;                 // Dat chan D5 nhan du lieu cho ctht Mo
int htDong = 4;               // Dat chan D4 nhan du lieu cho ctht dong
int button_MoThuCong = 11;    // Dat chan D11 nhan du lieu tu nut nhan button Mo
int button_DungThuCong = 12;  // Dat chan D12 nhan du lieu tu nut nhan button Dung
int button_DongThuCong = 8;   // Dat chan D8 nhan du lieu tu nut nhan button Dong
char in;                      // Bien nhan du lieu tu app thong qua module bluetooth
int buttonPushCounter = 0;    // Khoi tao bien So lan nut duoc nhan
int buttonState = 0;          // Trang thai hien tai cua nut 
int lastButtonState = 0;      // Trang thai nut truoc do


void setup() {
  // cai dat cac chan truyen doc du lieu tuong ung
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(htDong, INPUT);
  pinMode(htMo, INPUT);
  pinMode(buttonCounterPin,INPUT);
  pinMode(button_MoThuCong, INPUT);
  pinMode(button_DungThuCong, INPUT);
  pinMode(button_DongThuCong, INPUT);
  
   // cai dat toc do truyen cho hc06 va uno
  Serial.begin(9600);
  hc06.begin(9600);
}

//  Cai dat toc do phu hop cho dong co
void Dong(){
  analogWrite(in1, 70);
  digitalWrite(in2, LOW);
}

void Mo(){
  digitalWrite(in1,LOW);
  analogWrite(in2,70);
}

void Dung(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
}


void loop() {
  buttonState = digitalRead(buttonCounterPin);         //Doc gia tri hien tai cua nut nhan thay doi che do

      // So sanh voi gia tri nut bam truoc do
  if (buttonState != lastButtonState){
    if (buttonState == HIGH ){
      // Hien tai, nut dang duoc nhan thi buttonPushCounter tang them 1 don vi
      buttonPushCounter++;
    }
    else {
      //Luc nay, trang thai nut nhan la LOW, nen truoc do chac chan la nut nhan dang duoc nhan
    }
  }
  lastButtonState = buttonState;    //Luu trang thai lai cho nhung lan tiep theo

  

    // Ban dau, mai che dat o che do TU DONG, va cu moi lan nhan nut, che do TUDONG/THUDONG se thay doi qua lai
  if ( buttonPushCounter % 2 == 0){
    Serial.println(" ====  CHE DO TU DONG  ====  ");

    // cai dat cac ky tu de so sanh voi cac gia tri tra ve cua module bluetooth
    char dong = '0';
    char mo = '1';
    char dung = '4';
    char tat = '2';
    if (hc06.available()){  // khi co du lieu can doc tu module bluetooth
      in = hc06.read(); // doc du lieu tu mudule de bat dau dieu khien bang app

    // Khi khong bam nut tat module bluetooth
      while (in != tat){

        // doc gia tri hien tai cua hai cong tac hanh trinh
        int isDong = digitalRead(htDong);
        int isMo = digitalRead(htMo);
        if (hc06.available()){
          in = hc06.read();   // doc du lieu dieu khien tu app
        }
          
          // khi dang dong va nut dieu khien tren app la dong thi dung
        if (isDong == 0 && (in == dong)){
          while (true){
            Dung();

            // khi nhan duoc mot tin hieu moi khac dong
            if (hc06.available()){
              in = hc06.read();
              if(in != dong){
                break;
              } 
            }
          }
        }

        // tuong tu nhu dong
        else if (isMo == 0 && (in == mo)){
          while (true){
            Dung();
            if (hc06.available()){
               in = hc06.read();
              if (in != mo){
                break;
              }  
            }
          }   
        }
        // khi tin hieu la dong thi Dong;
        else if (in == dong) Dong();

        // khi tin hieu la mo thi Mo
        else if (in == mo) Mo();

        // khi tin hieu la dung thi Dung
        else if (in == dung) Dung();    
    }
  }
  
  // Che do tu dong
  else{
    int isDong = digitalRead(htDong); // doc gia tri cua ctht dong
    int isMo = digitalRead(htMo);
    int data = analogRead(A0);    // doc gia tri cam bien anh sang tu chan A0
      // Neu mai dang dong
    if (isDong == 0){
      if (data > 500) Mo();   // Troi mat thi Mo man
      else Dung();    // sang thi dung
    }
      // Neu man dang mo
    else if (isMo == 0) { 
      if (data <= 500) Dong();  // Troi sang thi dong man
      else Dung();  // troi mat thi dung
    }
    else if (data < 500) Dong();    // neu troi sang thi dong
    else if (data >= 500) Mo();     // neu troi toi thi mo
    }
  }

      // CHE DO THU CONG
  else {
     Serial.println(" ====  CHE DO THU CONG  ====  "); 
     // Doc lien tuc trang thai cua ba nut nhan thu cong
     int button_MoThuCong_State = digitalRead(button_MoThuCong);      
     int button_DungThuCong_State = digitalRead(button_DungThuCong);
     int button_DongThuCong_State = digitalRead(button_DongThuCong);

         // Nut nhan Mo mai duoc nhan thi...
     if ( button_MoThuCong_State == 1){
            if ( digitalRead(htMo) != 0 ){  
              Mo();
            }
            else Dung();
          }
          
          // Nut nhan Dong mai duoc nhan thi...
     else if (button_DongThuCong_State == 1){
            if(digitalRead(htDong) != 0){
              Dong();
            } 
            else Dung();
          }

          //Neu nhan nut Dung...
     else if (button_DungThuCong_State == 1){
            Dung();
          }
      }
  }



  
  
