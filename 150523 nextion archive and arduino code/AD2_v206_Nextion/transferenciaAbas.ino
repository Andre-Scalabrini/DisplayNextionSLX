void bateriacarregando(){
  Nextion.println("bateriacarregando.pic");
  Oxff();
}

void sembateria(){
  Nextion.println("sembateria.pic");
  Oxff();
  delay(50);
  Nextionln("nada.pic");
  Oxff();
  delay(50);
  //como o nome já diz, o aparelho ta sem carga
}

void niveldabateria(){
  //mexer a barrinha de acordo com a porcentagem
}

void simbolowifi(){

  if(wifi == ok){
    Nextion.println("simbolowifi.pic");
    Oxff();
  }else{
    Nextion.println("nada.pic");
    Oxff();
  }
  //vai mostrar o simbolo do wifi ou não
}

void bolhanivel(){
  //localização e design bolha nivel
}

void textoinicializacao(){
  // printa o texto de inicialização no display
}

void printagraus(){
  //printa no display os graus
}

void setinhasnivel(){
  //printa as setinhas do nível
}

void configangulo(){
  //só segue o nome
}

void configdefabrica(){
  //só segue o nome
}

void alteracaodelado(){
  //só segue o nome
}

void pagcamber(){
  //só segue o nome
}

void pagsalvar(){
  //só segue o nome
}

void pagcaster(){
  //só segue o nome
}

void pagzerar(){
  //só segue o nome
}

void saindodomenu(){
  //só segue o nome
}

void saindodoajuste(){
  //só segue o nome
}

void laserligado(){
  //só segue o nome
}

void laserdesligado(){
  //só segue o nome
}

void parametrodabateria8v(){
  //sei la oq isso significa mas tamo nois
}

void inicial(){
  //só segue o nome
}

void textoinicial(){
  //texto 1
  //texto 2
  //texto 3 (1 e 2 juntos)
}

void iniciopadraoslx(){
  //só segue o nome
}

void tecladoconfig(){
  //só segue o nome
}

void sensibilidadeconfig(){
  //só segue o nome
}

void calculodekpiconfig(){
  //só segue o nome
}

void configdeangulodegiro(){
  //só segue o nome
}

void configlado(){
  //só segue o nome
}

void modododisplayconfig(){
  //só segue o nome
}

void configtempodolaser(){
  //só segue o nome
}

void configtempododisplay(){
  //só segue o nome
}

void canalwificonfig(){
  //só segue o nome
}

void modospoilerconfig(){
  //só segue o nome
}

void mostrarindicadorbateria(){
  //só segue o nome
}

void nivelarmaquina(){
  //só segue o nome
}

void camberdenovo(){
  //só segue o nome
}

void niveleobracoousalveparaprosseguir(){
  //só segue o nome
}

void gireopneuparadentro(){

}

void retornea0egireparafora(){
  //só segue o nome
}

void kpiconfigoneoff(){
  //só segue o nome
}

void bateriabaixa(){
  //só segue o nome
}

void Oxff(){
  Nextion.write(0xff);
  Nextion.write(0xff);
  Nextion.write(0xff);
}