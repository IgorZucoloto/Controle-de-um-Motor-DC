#include <Keypad.h>
#include <LiquidCrystal.h>


volatile int pulsos;
int voltas = 0;
unsigned int pulsos_por_volta = 16;

 //Variáveis Globais Código para Cálculo rpm --- 
int   rpm;
 volatile byte pulsos_;
unsigned long timeold;

int IN1 = 2 ;
int IN2 = 4 ;
int velocidadeA = 3;
int velocidade = 0;

  

  
//Tamanha do teclado
const byte ROWS = 4; 
const byte COLS = 4; 
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
                        };

byte rowPins[ROWS] = {6, 7, 8, 9};
byte colPins[COLS] = {10, 11, 12, 13};
Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

LiquidCrystal lcd(A0,A1,A2,A3,A4,A5);//LCD
const String senhac = "123"; //SENHA CORRETA
String senha = "";




void contador()
{
  //Toda vez que é chamada conta mais um pulso.
pulsos_++;      
}




void setup()
{ 
  
 

  //Interrupção 0 - pino digital 13
  //Função para o sensor de velocidade
  attachInterrupt(digitalPinToInterrupt(13), contador, RISING);
  

  //Saidas da ponte H
pinMode(IN1,OUTPUT);
pinMode(IN2,OUTPUT);
pinMode(velocidadeA,OUTPUT); 
  
 
  //Monitor Serial
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.clear();
  Serial.println("Digite a senha!");
  
  

  
  //LCD
  

  lcd.setCursor(2,0);
  lcd.print("Projeto bea");
  lcd.setCursor(2,1);
  lcd.print("IGU-BEA");
  delay (3500);
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Igu ama bea?:");
  lcd.setCursor(0,1);
  lcd.print("(*)Limpar  (#)Ok"); 
}

//Teste da entrada (para checar se a senha está correta)




void loop()
{
  
  
  
  //Código responsável pela transformação para Rpm
   if (millis() - timeold >= 1000)
  {
    //Desabilita interrupcao durante o calculo
    detachInterrupt(0);
    rpm = (60 * 1000 / pulsos_por_volta ) / (millis() - timeold) * pulsos_;
    timeold = millis();
    pulsos_ = 0;

    //Mostra o valor de RPM no serial monitor
    Serial.print("RPM = ");
    Serial.println(rpm, DEC);
    //Habilita interrupcao
    attachInterrupt(0, contador, RISING);
  }
  
  
  //Funcionalidade de teclas
  char tecla = kpd.getKey(); //Confere se alguma tecla foi pressionada
  
   if (tecla){
    switch(tecla) 
    {
      case 'A':
      //Velocidade baixa de rotação do motor
      analogWrite(velocidadeA,80);

      //Comandos para mostrar no lcd 
      lcd.setCursor(9,1);
      lcd.print("A");
      delay(900);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Sim");
      delay(2000);
      lcd.clear();
      lcd.setCursor(0,0);

      lcd.print("Insira a Tecla");
        lcd.setCursor(0,1);
        lcd.print("Desejada:");
     
     
      break; 
      

      case 'B':
      
      //Velocidade Média do motor
      analogWrite(velocidadeA,150);


      //Comandos para mostrar no lcd 
      lcd.setCursor(9,1);
      lcd.print("B");
      delay(900);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Muitoo");
      delay(2000);
      lcd.clear();
      
      lcd.print("Insira a Tecla");
      lcd.setCursor(0,1);
      lcd.print("Desejada:");
      
      break;



      case 'C':
      //Velocidade alta de rotação do motor
      analogWrite(velocidadeA,230);
     //Comandos para mostrar no lcd 
      lcd.setCursor(9,1);
      lcd.print("C");
      delay(900);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Obvio ");
      delay(2000);
      lcd.clear();

      lcd.print("Insira a Tecla");
      lcd.setCursor(0,1);
      lcd.print("Desejada:");



      //Velocidade baixa de rotação do motor
  
       break; 


      case 'D':
      //Alterar o sentido da rotação
      digitalWrite(IN1,LOW);
      digitalWrite(IN2,LOW);

      lcd.setCursor(9,1);
      lcd.print("D");
      delay(900);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Inverter a rotação");
      delay(2000);
      lcd.clear();
      
      lcd.print("Insira a Tecla");
      lcd.setCursor(0,1);
      lcd.print("Desejada:");


       break; 
      
      
      
      
      //Teclado númérico que será usado somente para a senha
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      
      //Armazenamento da senha
      lcd.clear();
      senha += tecla; //armazena a sequência de teclas pressionadas
      Serial.print("Senha: ");
      Serial.println(senha);
      lcd.setCursor(1,0);
      lcd.print("Digite a Senha:");
      lcd.setCursor(0,1);
      lcd.print(senha);
      break;
     
     //Se a tecla * for pressionada a tela é limpa
     case '*':
      senha = "";
      Serial.println("Senha: "); // Reaparece a mensagem para a senha ser inserida 
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("Digite a Senha:");
     break;
     
     //OK (Verifica se a senha está correta)
     case'#':
      if(senha == senhac) // Se a senha inserida for igual a correta
      {
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("reposta certa!");//Mensagem que aparecerá na tela
        lcd.setCursor(6,1);
        lcd.print("YAY!");
        delay(1000);
        lcd.clear();
        lcd.print("Insira a Tecla");
        lcd.setCursor(0,1);
        lcd.print("Desejada:");


        //Motor Liga somente após a senha correta
         digitalWrite(IN1,HIGH);
         digitalWrite(IN2,LOW);
        
      }
       else //Se a senha inserida for incorreta
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("resposta errada!");
        lcd.setCursor(1,1);
        lcd.print("Pressione (*)");
        senha = ""; //Limpa a tela para nova tentativa
        Serial.println("Senha: ");
      }
      break;   
   }
   }
  }