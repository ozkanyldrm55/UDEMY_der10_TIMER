#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

GPIO_InitTypeDef LED;

TIM_TimeBaseInitTypeDef TIMStructure;

NVIC_InitTypeDef NVICStructure;

void config(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);

	LED.GPIO_Mode = GPIO_Mode_OUT;
	LED.GPIO_OType = GPIO_OType_PP;
	LED.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	LED.GPIO_PuPd = GPIO_PuPd_NOPULL;
	LED.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOD,&LED);

	TIMStructure.TIM_ClockDivision = TIM_CKD_DIV4;//Elektrik tüketimiyle ilgili cube mx programýndan ayarladýk.prescalar degeriyle ilgilidir.
	TIMStructure.TIM_Period = 9999; /* sn=((periyot+1)*(prescaler+1)/(clock speed)) burada clock oranýný cube mx den bakarak buluruz.prescalar degeri de max 16 bit
	                                gore belirlenecegi icin ona gore bir deger girmeliyiz. */
	TIMStructure.TIM_Prescaler = 8399;
	TIMStructure.TIM_CounterMode = TIM_CounterMode_Up;//yukarý dogru saymasini sagladik.
	TIMStructure.TIM_RepetitionCounter = 0; //bayragýmýzýn her bir artýsta bir artmasýný sagladýk (referans manual 593 syf)

	TIM_TimeBaseInit (TIM4,&TIMStructure);


	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);//timer ile interrupt i birbirine baglamamiz icin bu kod gerekli

	TIM_Cmd(TIM4,ENABLE); //Timer birimimizi aktif ettik


	NVICStructure.NVIC_IRQChannel = TIM4_IRQn;//NVIC in hangi timer i yönetecegini belirledik.
	NVICStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//herhengi bir oncelik olmadigini belirttik. preemption priority = hazirlik onceligi
	NVICStructure.NVIC_IRQChannelSubPriority = 0x00;// Sub Priority = alt oncelik
	NVICStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVICStructure);

}

int main(void)
{
	config();

  while (1)
  {

  }
}

void TIM4_IRQHandler()//interrupt fonk yazarken bu þekilde yazma zorunlulugu vardýr.bayrak her kalktýgýnda kod buraya girecek
{
	GPIO_ToggleBits(GPIOD,GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);

	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);//kesme bayragýný tekrar aktif olabilmesi icin indirdik.
}


void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
  /* TODO, implement your code here */
  return;
}


uint16_t EVAL_AUDIO_GetSampleCallBack(void){
  /* TODO, implement your code here */
  return -1;
}
