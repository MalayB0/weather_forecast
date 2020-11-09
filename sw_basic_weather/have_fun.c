#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<wchar.h>
#include<locale.h>
#include<engine.h>
#include<matrix.h>

//노드 구조
struct node
{
	wchar_t *position;
	int date;
	int low_tem;
	int high_tem;
	int rain;
	int moi;
	struct node *next;
	struct node *prev;
	struct node *under;
};
struct node *head=0;


//새로운 노드생성
struct node *creatNode(wchar_t *p,int d,int l_t,int h_t,int r,int m)
{
	struct node *cur=(struct node*)malloc(sizeof(struct node));
	//노드의 내용물
	cur->position=p;
	cur->date=d;
	cur->low_tem=l_t;
	cur->high_tem=h_t;
	cur->rain=r;
	cur->moi=m;

	//노드의 link
	cur->next=0;
	cur->prev=0;
	cur->under=0;
	return cur;
}

//SLL추가 - 시간
void addtoSLL(struct node *temp,struct node *cur)
{
	while(temp->under!=0)
	{
		temp=temp->under;
	}
	temp->under=cur;
	return;
}

//DLL추가 - 지역
void addtoDLL(wchar_t *p,int d,int l_t,int h_t,int r,int m)
{
	struct node *cur=creatNode(p,d,l_t,h_t,r,m);
	if(head==0)
	{
		head=cur;
		return;
	}
	struct node *temp=head;
	while(temp->next!=0)
	{
		temp=temp->next;
	}
	if(wcscmp(temp->position,cur->position)==0)
	{
		addtoSLL(temp,cur);
		return;
	}
	temp->next=cur;
	cur->prev=temp;
}

//입력한 날짜 탐색및 출력
void findnode()
{
	int whatis;
	printf("날짜를입력하세요: ");
	scanf("%d",&whatis);

	struct node *temp=head;
	while(temp!=0)
	{
		while(temp!=0)
		{
			if(whatis==temp->date)
				wprintf(L"Local: %s Tem:%3d ~ %3d Rain:%3d Mois:%3d\n",temp->position,temp->low_tem,temp->high_tem,temp->rain,temp->moi);
			temp=temp->under;
		}
		if(head->next==0)
			break;
		head=head->next;
		temp=head;
	}
	return;
}

//구조체 모두출력
void showALL()
{
	struct node *temp=head;
	while(temp!=0)
	{
		while(temp!=0)
		{
			wprintf(L"%s %d %3d %3d %3d %3d\n",temp->position,temp->date,temp->low_tem,temp->high_tem,temp->rain,temp->moi);
			temp=temp->under;
		}
		if(head->next==0)
			break;
		head=head->next;
		temp=head;
	}
	return;
}

void date_tem()
{
   int asd;
   //기온
   printf("입력한 날짜:");
   scanf_s("%d", &asd);
   if (asd == 0) return;
   //for (int i = 0; i < 4; i++) printf("%d", input_date[i]);
   struct node* temp = head;
   struct node* cur = head;
   int save = 0;
   while (cur != 0)
   {
      wprintf(L"      지역: %s\n\n", temp->position);
      int cnt = 0;
      int s_high_tem = 0;
	  int s_low_tem = 0;
      int s_rain = 0;
	  int s_moi = 0;
      while (temp != 0)
      {
         save = temp->date;
         save = save % 10000;
         if (save == asd)
         {
            printf("날짜: %d ", temp->date);
            printf("    기온: %d~%dºC ", temp->low_tem,temp->high_tem);
            printf("    강수량: %dmm ", temp->rain);
			printf("    습도: %d\n", temp->moi);
            cnt++;
            s_low_tem = s_low_tem + temp->low_tem;
			s_high_tem = s_high_tem + temp->high_tem;
            s_rain = s_rain + temp->rain;
			s_moi = s_moi + temp->moi;
         }
         temp = temp->under;
      }
      s_low_tem = s_low_tem / cnt;
      s_high_tem = s_high_tem / cnt;
      s_rain = s_rain / cnt;
	  s_moi = s_moi / cnt;
      printf("-----------------------------\n평균 기온 : %d~%dºC", s_low_tem,s_high_tem); 
	  printf("    평균 강수: %dmm",s_rain);
	  printf("    평균 습도: %d",s_moi);
      printf("\n-----------------------------\n");
      cur = cur->next;
      temp = cur;
   }
   date_tem();
   return;
}


//매트랩호출하여 인천,서울,양평,홍천,강릉지역의 계절별 온도,강수확률 figure생성
void showGraph(int n,int cnt,int a,int b,int c,int d)
{
	Engine *pEngine;

	int g[4]={a,b,c,d};
	mxArray *mxMatrix, *mxDet;
	double *pmxMatrix, *pDet;

	if(!(pEngine=engOpen(NULL)))
	{
		printf("Can not open the MATLAB engine.\n");
	}

	//배열 g의 내용물을 매트랩의 배열로 복사 
	mxMatrix=mxCreateDoubleMatrix(4,1,mxREAL);
	pmxMatrix=mxGetPr(mxMatrix);
	for(int i=0;i<4;i++) 
	{
		*((pmxMatrix)+i)=g[i];
	}

	engPutVariable(pEngine,"M",mxMatrix);
	//지역별 figure생성
	

	//최대,최소,강수확률 그리기
	if(n==3)
	{
		if(cnt==1)
			engEvalString(pEngine,"figure(6),set(figure(6),'OuterPosition',[1000,50,260,350]),ylabel('온도'),suptitle('강릉')");
		else if(cnt==2)
			engEvalString(pEngine,"figure(7),set(figure(7),'OuterPosition',[250,50,260,350]),ylabel('온도'),suptitle('서울')");
		else if(cnt==3)
			engEvalString(pEngine,"figure(8),set(figure(8),'OuterPosition',[0,50,260,350]),ylabel('온도'),suptitle('인천')");
		else if(cnt==4)
			engEvalString(pEngine,"figure(9),set(figure(9),'OuterPosition',[500,50,260,350]),ylabel('온도'),suptitle('양평')");
		else if(cnt==5)
			engEvalString(pEngine,"figure(10),set(figure(10),'OuterPosition',[750,50,260,350]),ylabel('온도'),suptitle('홍천')");
	
		engEvalString(pEngine,"bar(M,'c'),hold on,legend('강수확률'),ylim([0 40]),set(gca,'XTickLabel',{'봄','여름','가을','겨울'}),grid on");
	}
	else if(n==4)
	{
		if(cnt==1)
			engEvalString(pEngine,"figure(1),set(figure(1),'OuterPosition',[1000,50,260,350]),ylabel('강수량'),suptitle('강릉')");
		else if(cnt==2)
			engEvalString(pEngine,"figure(2),set(figure(2),'OuterPosition',[250,50,260,350]),ylabel('강수량'),suptitle('서울')");
		else if(cnt==3)
			engEvalString(pEngine,"figure(3),set(figure(3),'OuterPosition',[0,50,260,350]),ylabel('강수량'),suptitle('인천')");
		else if(cnt==4)
			engEvalString(pEngine,"figure(4),set(figure(4),'OuterPosition',[500,50,260,350]),ylabel('강수량'),suptitle('양평')");
		else if(cnt==5)
			engEvalString(pEngine,"figure(5),set(figure(5),'OuterPosition',[750,50,260,350]),ylabel('강수량'),suptitle('홍천')");
	
		engEvalString(pEngine,"bar(M,'b'),hold on,legend('총 강수량'),set(gca,'XTickLabel',{'여름','겨울'}),grid on");

	}

	else
	{
		if(cnt==1)
			engEvalString(pEngine,"figure(1),set(figure(1),'OuterPosition',[1000,400,260,350]),ylabel('온도'),suptitle('강릉')");
		else if(cnt==2)
			engEvalString(pEngine,"figure(2),set(figure(2),'OuterPosition',[250,400,260,350]),ylabel('온도'),suptitle('서울')");
		else if(cnt==3)
			engEvalString(pEngine,"figure(3),set(figure(3),'OuterPosition',[0,400,260,350]),ylabel('온도'),suptitle('인천')");
		else if(cnt==4)
			engEvalString(pEngine,"figure(4),set(figure(4),'OuterPosition',[500,400,260,350]),ylabel('온도'),suptitle('양평')");
		else if(cnt==5)
			engEvalString(pEngine,"figure(5),set(figure(5),'OuterPosition',[750,400,260,350]),ylabel('온도'),suptitle('홍천')");
		
		if(n==1)
			engEvalString(pEngine,"plot(M,'r'),hold on,ylim([-10 40]),set(gca,'XTickLabel',{'봄','여름','가을','겨울'}),grid on");
		else if(n==2)
			engEvalString(pEngine,"plot(M,'b'),legend('최대온도','최소온도')");
		
	}


}


//시즌별 최대,최소 온도, 강수확률의 평균
void seasonMean()
{
	int cnt=0;
	struct node *temp=head;
	
	//지역DLL 이동
	while(temp!=0)
	{
		//계절별 온도,강수확률 초기화
		int spring_cnt=0;
		int spring_low_temp=0;
		int spring_high_temp=0;
		int spring_rain_cnt=0;

		int summer_cnt=0;
		int summer_low_temp=0;
		int summer_high_temp=0;
		int summer_rain_cnt=0;
		int summer_rain=0;

		int fall_cnt=0;
		int fall_low_temp=0;
		int fall_high_temp=0;
		int fall_rain_cnt=0;

		int winter_cnt=0;
		int winter_low_temp=0;
		int winter_high_temp=0;
		int winter_rain_cnt=0;
		int winter_rain=0;

		wprintf(L"%s \n",temp->position);
		printf("----------------------------------------\n");
		
		//날짜SLL 이동
		while(temp->under!=0)
		{
			int month=(temp->date%10000)/100;
			
			if(month==3||month==4||month==5)
			{
				spring_cnt++;
				spring_low_temp += temp->low_tem;
				spring_high_temp += temp->high_tem;
				if(temp->rain!=0)
					spring_rain_cnt++;
			}
			else if(month==6||month==7||month==8)
			{
				summer_cnt++;
				summer_low_temp += temp->low_tem;
				summer_high_temp += temp->high_tem;
				summer_rain += temp->rain;
				if(temp->rain!=0)
					summer_rain_cnt++;
			}
			else if(month==9||month==10||month==11)
			{
				fall_cnt++;
				fall_low_temp += temp->low_tem;
				fall_high_temp += temp->high_tem;
				if(temp->rain!=0)
					fall_rain_cnt++;
			}
			else if(month==12||month==1||month==2)
			{
				winter_cnt++;
				winter_low_temp += temp->low_tem;
				winter_high_temp += temp->high_tem;
				winter_rain += temp->rain;
				if(temp->rain!=0)
					winter_rain_cnt++;
			}
			temp=temp->under;
		}
		printf("봄   - | 온도:%3d ~%3d | 강수확률: %d%% |\n",spring_low_temp/spring_cnt,spring_high_temp/spring_cnt,spring_rain_cnt*100/spring_cnt);
		printf("여름 - | 온도:%3d ~%3d | 강수확률: %d%% |\n",summer_low_temp/summer_cnt,summer_high_temp/summer_cnt,summer_rain_cnt*100/summer_cnt);
		printf("가을 - | 온도:%3d ~%3d | 강수확률: %d%% |\n",fall_low_temp/fall_cnt,fall_high_temp/fall_cnt,fall_rain_cnt*100/fall_cnt);
		printf("겨울 - | 온도:%3d ~%3d | 강수확률: %d%% |\n\n",winter_low_temp/winter_cnt,winter_high_temp/winter_cnt,winter_rain_cnt*100/winter_cnt);
		
		//printf("여름 - | 온도:%3d ~%3d | 총 강수량: %dmm |\n",summer_low_temp/summer_cnt,summer_high_temp/summer_cnt,summer_rain);
		//printf("겨울 - | 온도:%3d ~%3d | 총 강수량: %dmm |\n\n",winter_low_temp/winter_cnt,winter_high_temp/winter_cnt,winter_rain);
		

		int sp_l=spring_low_temp/spring_cnt;
		int sp_h=spring_high_temp/spring_cnt;
		int sp_r=spring_rain_cnt*100/spring_cnt;
		int su_l=summer_low_temp/summer_cnt;
		int su_h=summer_high_temp/summer_cnt;
		int su_r=summer_rain_cnt*100/summer_cnt;
		int fa_l=fall_low_temp/fall_cnt;
		int fa_h=fall_high_temp/fall_cnt;
		int fa_r=fall_rain_cnt*100/fall_cnt;
		int wi_l=winter_low_temp/winter_cnt;
		int wi_h=winter_high_temp/winter_cnt;
		int wi_r=winter_rain_cnt*100/winter_cnt;
		cnt++;	
		
		//showGraph(4,cnt,summer_rain,winter_rain);
		showGraph(3,cnt,sp_r,su_r,fa_r,wi_r);
		showGraph(1,cnt,sp_h,su_h,fa_h,wi_h);
		showGraph(2,cnt,sp_l,su_l,fa_l,wi_l);
		
		if(head->next==0)
			break;
		head=head->next;
		temp=head;
	}

	return;
}



//불러온 기상데이터에서 필요한 정보wchar로 불러와 숫자만 int형으로 파씽
void Parsing(wchar_t *s)
{
	//txt파일에서의 각 정보들의 위치파악
	int len=wcslen(s);
	int firstSpace=2;

	int beforedate=firstSpace+1;
	int secondSpace=beforedate+8;
	
	int beforehigh_tem=secondSpace+1;
	int thirdSpace=beforehigh_tem+3;
	
	int beforelow_tem=thirdSpace+1;
	int fourthSpace=beforelow_tem+3;
	
	int beforerain=fourthSpace+1;
	int fifthSpace=beforerain+3;
	
	int beforemoistrue=fifthSpace+1;

	//지역 wchar
	wchar_t *pos=(wchar_t*)malloc((firstSpace)*sizeof(wchar_t)+1);
	int k = 0;
	for (int j = 0; j < firstSpace; j++, k++)
	{
		pos[k] = s[j];
	}
	pos[k] = L'\0';
	
	//날짜 wchar -> int
	wchar_t *date = (wchar_t*)malloc((secondSpace-beforedate)*sizeof(wchar_t)+1);
	k = 0;
	for (int j=beforedate;j<secondSpace;j++,k++)
	{
		date[k] = s[j];
	}
	date[k] = L'\0';
	int datei=_wtoi(date);
	
	//최고온도 wchar -> int
	wchar_t *high_tem = (wchar_t*)malloc((thirdSpace-beforehigh_tem) * sizeof(wchar_t)+1);
	k = 0;
	for (int j = beforehigh_tem; j < thirdSpace; j++, k++)
	{
		high_tem[k] = s[j];
	}
	high_tem[k] = L'\0';
	int high_temi=_wtoi(high_tem);

	//최저온도 wchar -> int
	wchar_t *low_tem = (wchar_t*)malloc((fourthSpace-beforelow_tem) * sizeof(wchar_t)+1);
	k = 0;
	for (int j = beforelow_tem; j < fourthSpace; j++, k++)
	{
		low_tem[k] = s[j];
	}
	low_tem[k] = L'\0';
	int low_temi=_wtoi(low_tem);

	//강수량 wchar -> int
	wchar_t *rain = (wchar_t*)malloc((fifthSpace-beforerain) * sizeof(wchar_t)+1);
	k = 0;
	for (int j = beforerain; j < fifthSpace; j++, k++)
	{
		rain[k] = s[j];
	}
	rain[k] = L'\0';
	int raini=_wtoi(rain);
	
	//습도 wchar -> int
	wchar_t *moi = (wchar_t*)malloc(((len)-beforemoistrue) * sizeof(wchar_t)+1);
	k = 0;
	for (int j = beforemoistrue; j < len; j++, k++)
	{
		moi[k] = s[j];
	}
	moi[k] = L'\0';
	int moii=_wtoi(moi);

	//6개의 데이터 노드로 추가
	addtoDLL(pos,datei,high_temi,low_temi,raini,moii);
}

int main()
{
	setlocale(LC_ALL,"Korean");
	//파일불러오기
	FILE *f=0;
	wchar_t pst[100];
	f=fopen("howhow.txt","rt");

	//파일불러오기실패
	if(f==0)
	{
		printf("unable open file\n");
		return -1;
	}
	//파일 읽어오기
	while(1)
	{
		if(fgetws(pst,100,f)==0)
			break;
		Parsing(pst);
	}
	//파일 불러오기 종료
	fclose(f);
	//시즌별 평균
	seasonMean();

	//입력한 날짜의 모든 지역의 데이터출력	
	//findnode();
}

