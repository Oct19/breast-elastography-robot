#include "spwm.h"
#include "motor.h"
#include "eesv3.h"
#include <math.h>
#include "can.h"
#include "usart2.h"

#if SYSTEM_SUPPORT_OS
#include "includes.h"                   //os Ê¹ÓÃ     
#endif

uint16_t  T8_integer_num;     //ÔÈËÙ½×¶ÎÂö³åÊýµÄ255µÄÕûÊý±¶
uint8_t   T8_remainder_num; //ÔÈËÙ½×¶ÎÂö³åÊýµÄ255µÄÕûÊý±¶ÓàÊý
uint8_t TIM8_STATUS=0;          //TIM3¶¨Ê±Æ÷¹¤×÷×´Ì¬±êÖ¾£¨0£º¿ÕÏÐ£»1£ºÃ¦£©
uint8_t T8Motor_Code=0;         //µç»ú´úºÅ£¬ÉèÖÃ³ÉÈ«¾Ö±äÁ¿
uint8_t T8RunModel=0;           //µÚ¶þ×éµç»úÔËÐÐ±êÖ¾Î»

uint16_t PREQ_TABBUR[31]={6400,6502,6810,7322,8038,8960,9424,10448,11472,12496,13520,14544,15568,16592,
//  17616,18640,19664,20688,21712,22736,23760,24784,25808,26832,27856,28880,30362,31078,31590,31898,32000}; 

/*------------------------------------------------------------------------------------
 * @function: static void PWM_GPIOC_Config(void)
 * @author  : Zhou Xiangping
 * @input   : Nothing 
 * @return  : Nothing 
 * @date    : 19-2-2017
 * @brief   : ³õÊ¼»¯¶¨Ê±Æ÷TIM3µÄPWMÊä³öÒý½Å
-------------------------------------------------------------------------------------*/
static void TIM8_PWM_RCR_Config(u16 arr,u16 psc)
{                            
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE); //TIM8Ê±ÖÓÊ¹ÄÜ
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);  //Ê¹ÄÜGPIOCÍâÉèÊ±ÖÓÊ¹ÄÜ                                                                         

  //ÉèÖÃ¸ÃÒý½ÅÎª¸´ÓÃÊä³ö¹¦ÄÜ,Êä³öTIM8 CH2µÄPWMÂö³å²¨ÐÎ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; //TIM8_CH2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //¸´ÓÃÍÆÍìÊä³ö
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

    TIM_TimeBaseStructure.TIM_Period = arr; //ÉèÖÃÔÚÏÂÒ»¸ö¸üÐÂÊÂ¼þ×°Èë»î¶¯µÄ×Ô¶¯ÖØ×°ÔØ¼Ä´æÆ÷ÖÜÆÚµÄÖµ     
    TIM_TimeBaseStructure.TIM_Prescaler =psc; //ÉèÖÃÓÃÀ´×÷ÎªTIMxÊ±ÖÓÆµÂÊ³ýÊýµÄÔ¤·ÖÆµÖµ  
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //ÉèÖÃÊ±ÖÓ·Ö¸î:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIMÏòÉÏ¼ÆÊýÄ£Ê½
    TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //¸ù¾ÝTIM_TimeBaseInitStructÖÐÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯TIMxµÄÊ±¼ä»ùÊýµ¥Î»
    TIM_ClearITPendingBit(TIM8,TIM_IT_Update);

    TIM_UpdateRequestConfig(TIM8,TIM_UpdateSource_Regular); /********* ÉèÖÃÖ»ÓÐ¼ÆÊýÒç³ö×÷Îª¸üÐÂÖÐ¶Ï ********/
//  TIM_SelectOnePulseMode(TIM8,TIM_OPMode_Single);/******* µ¥Âö³åÄ£Ê½ **********/

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //Ñ¡Ôñ¶¨Ê±Æ÷Ä£Ê½:TIMÂö³å¿í¶Èµ÷ÖÆÄ£Ê½2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //±È½ÏÊä³ö2Ê¹ÄÜ
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable; /****** ±È½ÏÊä³ö2NÊ§ÄÜ *******/
    TIM_OCInitStructure.TIM_Pulse = arr>>1; //ÉèÖÃ´ý×°Èë²¶»ñ±È½Ï¼Ä´æÆ÷µÄÂö³åÖµ
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //Êä³ö¼«ÐÔ:TIMÊä³ö±È½Ï¼«ÐÔ¸ß

    TIM_OC1Init(TIM8, &TIM_OCInitStructure);                //¸ù¾ÝTIM_OCInitStructÖÐÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯ÍâÉèTIMx
    TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable); //Ê¹ÄÜTIMxÔÚCCR1ÉÏµÄÔ¤×°ÔØ¼Ä´æÆ÷
    TIM_OC2Init(TIM8, &TIM_OCInitStructure); 
    TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable); //Ê¹ÄÜTIMxÔÚCCR2ÉÏµÄÔ¤×°ÔØ¼Ä´æÆ÷
    TIM_OC3Init(TIM8, &TIM_OCInitStructure);                
    TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable); //Ê¹ÄÜTIMxÔÚCCR3ÉÏµÄÔ¤×°ÔØ¼Ä´æÆ÷
    TIM_OC4Init(TIM8, &TIM_OCInitStructure);                
    TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable); //Ê¹ÄÜTIMxÔÚCCR4ÉÏµÄÔ¤×°ÔØ¼Ä´æÆ÷

    TIM_ARRPreloadConfig(TIM8, ENABLE); //Ê¹ÄÜTIMxÔÚARRÉÏµÄÔ¤×°ÔØ¼Ä´æÆ÷

    TIM_ITConfig(TIM8, TIM_IT_Update ,ENABLE);  //TIM8   Ê¹ÄÜ»òÕßÊ§ÄÜÖ¸¶¨µÄTIMÖÐ¶Ï

    NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_IRQn;  //TIM8ÖÐ¶Ï
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //ÏÈÕ¼ÓÅÏÈ¼¶1¼¶
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //´ÓÓÅÏÈ¼¶1¼¶
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQÍ¨µÀ±»Ê¹ÄÜ
    NVIC_Init(&NVIC_InitStructure);  //¸ù¾ÝNVIC_InitStructÖÐÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯ÍâÉèNVIC¼Ä´æÆ÷

    TIM_ClearITPendingBit(TIM8, TIM_IT_Update);  //Çå³ýTIMxµÄÖÐ¶Ï´ý´¦ÀíÎ»:TIM ÖÐ¶ÏÔ´
    TIM_Cmd(TIM8, ENABLE);  //Ê¹ÄÜTIM8                                    
}
/*------------------------------------------------------------------------------------
 * @function: PWM_OUT_Init(uint16_t arr,uint16_t psc)
 * @author  : Zhou Xiangping
 * @input   : arr£º×Ô¶¯ÖØ×°Öµ   psc£ºÊ±ÖÓÔ¤·ÖÆµ
 * @return  : Nothing 
 * @date    : 19-2-2017
 * @brief   : ³õÊ¼»¯TIM3¡TIM4µÄPWMÊä³ö¹¦ÄÜ
-------------------------------------------------------------------------------------*/
void TIM8_PWM_OUT_Init(uint16_t arr,uint16_t psc)
{
    TIM8_PWM_RCR_Config(arr, psc);
    TIM_SetCompare1(TIM8,0); //ÉèÖÃÍ¨µÀ1µÄÕ¼¿Õ±È50%
    TIM_SetCompare2(TIM8,0); //ÉèÖÃÍ¨µÀ2µÄÕ¼¿Õ±È50%
    TIM_SetCompare3(TIM8,0); //ÉèÖÃÍ¨µÀ3µÄÕ¼¿Õ±È50%
    TIM_SetCompare4(TIM8,0); //ÉèÖÃÍ¨µÀ4µÄÕ¼¿Õ±È50%
}
/*------------------------------------------------------------------------------------
 * @function: TIM3_Startup(uint8_t Channel,uint32_t frequency)
 * @author  : Zhou Xiangping
 * @input   : Channel£ºPWMÊä³öÍ¨µÀ1~4£»    Frequency£ºPWMÊä³öÆµÂÊ
 * @return  : Nothing 
 * @date    : 19-2-2017
 * @brief   : ÖØÐÂ×°ÔØ¶¨Ê±Æ÷TIM3µÄ¸÷Í¨µÀPWMÊä³öÆµÂÊ¼°Õ¼¿Õ±È
-------------------------------------------------------------------------------------*/
void TIM8_Startup(uint8_t Channel,uint32_t Frequency)
{
    u16 temp_arr=1000000/Frequency-1; 
    TIM_SetAutoreload(TIM8,temp_arr);// 
    switch(Channel)
    {
        case 1:
                    TIM_SetCompare1(TIM8,temp_arr>>1); //ÉèÖÃÍ¨µÀ1µÄÕ¼¿Õ±È50%
            break;
        case 2:
                    TIM_SetCompare2(TIM8,temp_arr>>1); //ÉèÖÃÍ¨µÀ2µÄÕ¼¿Õ±È50%
            break;
        case 3:
                    TIM_SetCompare3(TIM8,temp_arr>>1); //ÉèÖÃÍ¨µÀ3µÄÕ¼¿Õ±È50%       
            break;
        case 4: 
                    TIM_SetCompare4(TIM8,temp_arr>>1); //ÉèÖÃÍ¨µÀ4µÄÕ¼¿Õ±È50%   
            break;
        default: break;
    }   
    TIM_SetCounter(TIM8,0);//
    TIM_CtrlPWMOutputs(TIM8,ENABLE);    //MOE Ö÷Êä³öÊ¹ÄÜ£¨¸ß¼¶¶¨Ê±Æ÷Ê±±ØÒªµÄ»·½Ú£©
    TIM_Cmd(TIM8, ENABLE); //
}
/*------------------------------------------------------------------------------------
 * @function: TIM8_UP_IRQHandler(void) //TIM8-RCRÒç³öÖÐ¶Ï·þÎñ³ÌÐò
 * @author  : Zhou Xiangping
 * @input   : Nothing
 * @return  : Nothing 
 * @date    : 19-2-2017
 * @brief   : (µÚÒ»×éµç»ú¿ØÖÆËã·¨ÊµÏÖº¯Êý)¶¨Ê±Æ÷TIM8Òç³öÖÐ¶Ï£¬´¦Àíµç»ú¿ØÖÆÐÅºÅSTEPµÄÂö³åÊä³öÆµÂÊ
                        : ¿ØÖÆµç»ú0¡¢1¡¢2¡¢3ÔË¶¯
-------------------------------------------------------------------------------------*/ 
void TIM8_UP_IRQHandler(void)
{
    static uint8_t Accel_num=0,Decel_num=STEP_NUMX;
    static uint32_t totall=0;
    static CanBus Can1; 

#ifdef SYSTEM_SUPPORT_OS        
    OSIntEnter();    
#endif
    if(TIM_GetITStatus(TIM8,TIM_FLAG_Update)!=RESET)
    {
        TIM_ClearITPendingBit(TIM8,TIM_FLAG_Update);
        if(TIM8_STATUS==1)
        {
            if(Accel_num<STEP_NUMX) 
            {
                Accel_num++;
                totall+=SUBD_NUM;   
                switch(T8Motor_Code)
                {
                    case 0: TIM8_Startup(1,PREQ_TABBUR[Accel_num]);
                    break;
                    case 1: TIM8_Startup(2,PREQ_TABBUR[Accel_num]);
                    break;
                    case 2: TIM8_Startup(3,PREQ_TABBUR[Accel_num]);
                    break;
                    case 3: TIM8_Startup(4,PREQ_TABBUR[Accel_num]);
                    break;  
                    default:break;                          
                }   
//              u2_printf("\r\nT8[%d]= %d",Motor_Code,PREQ_TABBUR[Decel_num]);
            }else if(T8_integer_num>0||T8_remainder_num>0)
            {
                if(T8_integer_num!=0)
                {
                    T8_integer_num--;
                    TIM8->RCR=RCR_VAL;
                    totall+=RCR_VAL;
                }else if(T8_remainder_num!=0)
                {
                    totall+=T8_remainder_num;
                    TIM8->RCR=T8_remainder_num;
                    T8_remainder_num=0;                                 
                }
//              u2_printf("\r\nT8[%d]= %d",T8Motor_Code,PREQ_TABBUR[Decel_num]);
            }else if(T8RunModel >= 1)
            {
//               switch(Motor_Code)
//                  {
//                      case 0: T1RunModel=ScanSensor_EESV3(SensorNum);
//                      break;
//                      case 1: T1RunModel=ScanSensor_EESV3(SensorNum);
//                      break;
//                      case 2: T1RunModel=ScanSensor_EESV3(SensorNum);
//                      break;
//                      case 3: T1RunModel=ScanSensor_EESV3(SensorNum);
//                      break;  
//                      default:break;                                  
//                  }
                u2_printf("\r\nT8RunModel= %d",T8RunModel);
                T8RunModel=ScanSensor_EESV3(1);
                u2_printf("\r\nT8RunModel= %d",T8RunModel);
                TIM8->RCR=SUBD_NUM; 
            }else if(Decel_num>0) //¼õËÙ½×¶Î
                {
                    TIM8->RCR=SUBD_NUM-1;
                    Decel_num--;
                    totall+=SUBD_NUM;   
                    switch(T8Motor_Code)
                    {
                        case 0: TIM8_Startup(1,PREQ_TABBUR[Decel_num]);
                        break;
                        case 1: TIM8_Startup(2,PREQ_TABBUR[Decel_num]);
                        break;
                        case 2: TIM8_Startup(3,PREQ_TABBUR[Decel_num]);
                        break;
                        case 3: TIM8_Startup(4,PREQ_TABBUR[Decel_num]);
                        break;  
                        default:break;                              
                    }   
//              u2_printf("\r\nT8[%d]= %d",T8Motor_Code,PREQ_TABBUR[Decel_num]);
                }else goto out;
            }else
            {
out:
//           u2_printf("\r\n totall=%d\r\n",totall);
                totall=0;
                TIM8_STATUS=0;           //±êÖ¾Ò»¸ö¹¤×÷×ªÌ¬Íê³É
                Accel_num=0;
                Decel_num=STEP_NUMX;
                TIM_CtrlPWMOutputs(TIM8,DISABLE);   //MOE Ö÷Êä³ö¹Ø±Õ
                TIM_Cmd(TIM8, DISABLE);  //¹Ø±ÕTIM8
                MOTOR_STOP0;
                TIM_SetCompare1(TIM8,0); //ÉèÖÃÍ¨µÀ1µÄÕ¼¿Õ±È50%
                TIM_SetCompare2(TIM8,0); //ÉèÖÃÍ¨µÀ2µÄÕ¼¿Õ±È50%
                TIM_SetCompare3(TIM8,0); //ÉèÖÃÍ¨µÀ3µÄÕ¼¿Õ±È50%
                TIM_SetCompare4(TIM8,0); //ÉèÖÃÍ¨µÀ4µÄÕ¼¿Õ±È50%
                switch(T8Motor_Code)
                {
                    case 0:         
                        Can1.CheckCode=(0xc0+T8Motor_Code+0x55+0xff+0xff+0xff+0xff)&0x00FF;
                        CAN1SetData(Can1.tx_buffer,0xc0,T8Motor_Code,0x55,0xff,0xff,0xff,0xff,Can1.CheckCode);
                    break;
                    case 1:
                        Can1.CheckCode=(0xc0+T8Motor_Code+0x55+0xff+0xff+0xff+0xff)&0x00FF;
                        CAN1SetData(Can1.tx_buffer,0xc0,T8Motor_Code,0x55,0xff,0xff,0xff,0xff,Can1.CheckCode);
                    break;
                    case 2:
                        Can1.CheckCode=(0xc0+T8Motor_Code+0x55+0xff+0xff+0xff+0xff)&0x00FF;
                                    CAN1SetData(Can1.tx_buffer,0xc0,T8Motor_Code,0x55,0xff,0xff,0xff,0xff,Can1.CheckCode);
                    break;
                    case 3: 
                        Can1.CheckCode=(0xc0+T8Motor_Code+0x55+0xff+0xff+0xff+0xff)&0x00FF;
                        CAN1SetData(Can1.tx_buffer,0xc0,T8Motor_Code,0x55,0xff,0xff,0xff,0xff,Can1.CheckCode);
                    break;  
                    default:break;                              
                }   
                CAN1_send_message(Can1.tx_buffer,8);                
            }           
    }
#ifdef SYSTEM_SUPPORT_OS    //Èç¹ûÊ±ÖÓ½ÚÅÄÊý¶¨ÒåÁË,ËµÃ÷ÒªÊ¹ÓÃucosIIÁË.
    OSIntExit();                                             
#endif
}
uint8_t Locate_Rle_FullStep2(long num,uint8_t dir,uint8_t MOTOR,uint8_t Model)
{
    long StepNum=0;
    long T8Uniform_NUM; 
//  u2_printf("\r\nStepNum=%d!!\r\n",StepNum);
    if(num<=0) //ÊýÖµÐ¡µÈÓÚ0 ÔòÖ±½Ó·µ»Ø
    {
        u2_printf("\r\n The num should be greater than zero!!\r\n");
        return 1;
    }
    /*************************************************************************/
    if(TIM8_STATUS)
    {
        u2_printf("\r\nT8 last time pulses is not send finished!\r\n");
        return 2;
    }else{
        T8RunModel=Model;
        T8Motor_Code=MOTOR; 
        StepNum=num*SUBD_NUM;//ËùÐèÂö³åÊý
        T8Uniform_NUM=StepNum-(STEP_NUMX*2*SUBD_NUM);       
        T8_integer_num=T8Uniform_NUM/(RCR_VAL+1);
        T8_remainder_num=T8Uniform_NUM%(RCR_VAL+1);
        TIM8->RCR=SUBD_NUM-1;           
        switch(MOTOR)
        {
            case 0:
                    if(dir==1)
                    {
                        MOTOR0_DIR =1; //È·¶¨µç»úÐý×ª·½Ïò
                    }else
                    {
                        MOTOR0_DIR =0;
                    }
                    MOTOR0_OE;
                    u2_printf("MOTOR0_DIR=%d!\r\n",MOTOR0_DIR);
                    TIM8_Startup(1,PREQ_TABBUR[0]); //ÉèÖÃÊä³öÆðÌøÆµÂÊ
                    break;
            case 1:
                    if(dir==1) MOTOR1_DIR =1; //È·¶¨µç»úÐý×ª·½Ïò
                    else             MOTOR1_DIR =0;
                    MOTOR1_OE;
                    TIM8_Startup(2,PREQ_TABBUR[0]); //ÉèÖÃÊä³öÆðÌøÆµÂÊ
                    break;
            case 2:
                    if(dir==1) MOTOR2_DIR =1; //È·¶¨µç»úÐý×ª·½Ïò
                    else             MOTOR2_DIR =0;
                    TIM8_Startup(3,PREQ_TABBUR[0]); //ÉèÖÃÊä³öÆðÌøÆµÂÊ
                    break;
            case 3:
                    if(dir==1) MOTOR3_DIR =1; //È·¶¨µç»úÐý×ª·½Ïò
                    else             MOTOR3_DIR =0;
                    MOTOR3_OE;  
                    TIM8_Startup(4,PREQ_TABBUR[0]); //ÉèÖÃÊä³öÆðÌøÆµÂÊ
            break;
            default:
                     break;             
        }   
        TIM8_STATUS=1;
    }
    return 0;
}



/*第一次写博客，图片和公式上传太折腾了！！！*/