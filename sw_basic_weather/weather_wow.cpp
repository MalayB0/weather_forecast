#include<stdlib.h>
#include<string.h>
#include<wchar.h>
#include<locale.h>
#include<stdio.h>
#include<math.h>
#include<engine.h>

struct node
{
   wchar_t *position;
   int date;
   int low_tem;
   int high_tem;
   int rain;
   int moi;
   struct node* up;
   struct node *under;
};

struct save {
   struct node* node_posi;
   struct save* next;
   struct save* prev;
};

struct node *head = 0;
struct node *tail = 0;

struct node *find_head = 0; // 비교하는 날의 전날의 시작.
struct node *find_tail = 0;

struct node *cmp_head = 0;
struct node *cmp_tail = 0;

struct save *save_head = 0;
struct save *save_tail = 0;

struct save *save_creat(struct node* n_posi)
{
   struct save* temp = (struct save*)malloc(sizeof(struct save));
   temp->node_posi = n_posi;
   temp->next = 0;
   temp->prev = 0;

   return temp;
}

void add_save_dll(struct node* n_posi)
{
   struct save* temp = save_creat(n_posi);
   if (save_head == 0)
   {
      save_head = temp;
      return;
   }
   else if (save_head != 0)
   {
      struct save* cur = save_head;
      while (cur->next != 0)
      {
         cur = cur->next;
      }
      cur->next = temp;
      temp->prev = cur;
      return;
   }
   else
   {
      printf("add_save_dll 문제");
      return;
   }
}

void delet_save_dll(struct save* unco_save)
{
   if (unco_save == save_head)
   {
      if (save_head->next != 0) 
      {
         save_head = save_head->next;
         save_head->prev = 0;
         free(unco_save);
         return;
      }
      else if (save_head->next == 0)
      {
         struct save* temp_h = save_head;
         free(temp_h);
         save_head = 0;
      }
   }
   else if (unco_save != save_head)
   {
      struct save* temp = unco_save->prev;
      temp->next = unco_save->next;
      if (unco_save->next == 0)
      {
         free(unco_save);
         return;
      }
      unco_save->next->prev = temp;
      if (save_head == unco_save)
      {
         save_head = 0;
      }
      free(unco_save);
      return;
   }
   else
   {
      printf("delet_save_dll error\n");
      return;
   }
}

//새로운 노드생성
struct node *creatNode(wchar_t *p, int d, int l_t, int h_t, int r, int m)
{
   struct node *cur = (struct node*)malloc(sizeof(struct node));
   //노드의 내용물
   cur->position = p;
   cur->date = d;
   cur->low_tem = l_t;
   cur->high_tem = h_t;
   cur->rain = r;
   cur->moi = m;
   //노드의 link
   cur->under = 0;
   cur->up = 0;
   return cur;
}

void add_under_dll(wchar_t *p, int d, int l_t, int h_t, int r, int m)
{
   struct node* temp = creatNode(p,d,l_t,h_t,r,m);
   tail = temp;
   if (head == 0)
   {
      head = temp;
      return;
   }
   struct node* cur = head;
   while (1)
   {
      if (cur->under == 0)
         break;
      cur = cur->under;
   }
   cur->under = temp;
   temp->up = cur;
   return;
}

void moi_parsing(wchar_t* asf)
{
   int len = wcslen(asf);
   int firstSpace = 4;

   int beforedate = firstSpace + 1;       //3
   int secondSpace = beforedate + 8;      //11

   int beforehigh_tem = secondSpace + 1;  //12
   int thirdSpace = beforehigh_tem + 3;   //15

   int beforelow_tem = thirdSpace + 1;    //16
   int fourthSpace = beforelow_tem + 3;   //17

   int beforerain = fourthSpace + 1;      //18
   int fifthSpace = beforerain + 3;       //21

   int beforemoistrue = fifthSpace +1;   //22
   //printf("%d\n", len);
                                 //지역 wchar
   wchar_t *pos = (wchar_t*)malloc((firstSpace) * sizeof(wchar_t) + 1);
   int k = 0;
   for (int j = 0; j < firstSpace; j++, k++)
   {
      pos[k] = asf[j];
   }
   pos[k] = L'\0';

   //날짜 wchar -> int
   wchar_t *date = (wchar_t*)malloc((secondSpace - beforedate) * sizeof(wchar_t) + 1);
   k = 0;
   for (int j = beforedate; j<secondSpace; j++, k++)
   {
      date[k] = asf[j];
   }
   date[k] = L'\0';
   int datei = _wtoi(date);

   //최저온도 wchar -> int
   wchar_t *low_tem = (wchar_t*)malloc((thirdSpace - beforehigh_tem) * sizeof(wchar_t) + 1);
   k = 0;
   for (int j = beforehigh_tem; j < thirdSpace; j++, k++)
   {
      low_tem[k] = asf[j];
   }
   low_tem[k] = L'\0';
   int low_temi = _wtoi(low_tem);

   //최저온도 wchar -> int
   wchar_t *high_tem = (wchar_t*)malloc((fourthSpace - beforelow_tem) * sizeof(wchar_t) + 1);
   k = 0;
   for (int j = beforelow_tem; j < fourthSpace; j++, k++)
   {
      high_tem[k] = asf[j];
   }
   high_tem[k] = L'\0';
   int high_temi = _wtoi(high_tem);

   //강수량 wchar -> int
   wchar_t *rain = (wchar_t*)malloc((fifthSpace - beforerain) * sizeof(wchar_t) + 1);
   k = 0;
   for (int j = beforerain; j < fifthSpace; j++, k++)
   {
      rain[k] = asf[j];
   }
   rain[k] = L'\0';
   int raini = _wtoi(rain);

   //습도 wchar -> int
   wchar_t *moi = (wchar_t*)malloc(((len)-beforemoistrue) * sizeof(wchar_t) + 1);
   k = 0;
   for (int j = beforemoistrue; j < len; j++, k++)
   {
      moi[k] = asf[j];
   }
   moi[k] = L'\0';
   int moii = _wtoi(moi);
   //printf("%d\n", datei);
   //printf("%d %d %d %d %d\n", datei, high_temi, low_temi, raini, moii);
   add_under_dll(pos, datei, low_temi, high_temi, raini, moii);

}
void show_under()
{
   struct node* temp = head;
   if (head == 0) {
      printf("no list\n");
      return;
   }
   while (temp != 0)
   {
      wprintf(L"%s %d %3d %3d %3d %3d\n", temp->position, temp->date, temp->low_tem, temp->high_tem, temp->rain, temp->moi);
      temp = temp->under;
   }
   return;
}

void show_under_sta()
{
   struct node* temp = find_head;
   if (find_head == 0) {
      printf("no list\n");
      return;
   }
   while (temp != 0)
   {
      wprintf(L"%s %d %3d %3d %3d %3d\n", temp->position, temp->date, temp->low_tem, temp->high_tem, temp->rain, temp->moi);
      temp = temp->under;
   }
   return;
}

void show_save(int input_date)
{
   int cnt = 1;

   int l_tem_s = 0; int h_tem_s = 0; int rain_s = 0; int moi_s = 0;
   struct save* temp = save_head;
   if (save_head == 0)
   {
      printf("no save list\n");
      return;
   }
   while (temp != 0)
   {
      struct node* cur = temp->node_posi;
      printf("%d, 높게 일치되는 일주일\n------------------------------------------------------------\n",cnt);
      for (int idx = 0; idx < 7; idx++)
      {
         printf("날짜: %d 최저기온: %d 최고기온: %d 강수량: %d 습도량: %d \n",cur->date,cur->low_tem,cur->high_tem,cur->rain,cur->moi);
         cur = cur->under;
      }
      printf("------------------------------------------------------------\n\n");
      printf("날짜: %d 최저기온: %d 최고기온: %d 강수량: %d 습도량: %d \n", cur->date, cur->low_tem, cur->high_tem, cur->rain, cur->moi);
      printf("\n");
      l_tem_s = l_tem_s + cur->low_tem;
      h_tem_s = h_tem_s + cur->high_tem;
      rain_s = rain_s + cur->rain;
      moi_s = moi_s + cur->moi;
      //printf("%d", cnt);
      //printf("습도 filter:");
      //printf("%d\n",temp->node_posi->date);
      cnt++;
      
      temp = temp->next;
   }
   cnt--;
   printf("\n-------------------------------------------------------------------------\n\n");
   printf("최종 예측결과 - %d: 최저기온: %d 최고기온: %d  강수량: %d 습도: %d\n",input_date,l_tem_s/cnt,h_tem_s/cnt,rain_s/cnt,moi_s/cnt);
   printf("\n-------------------------------------------------------------------------\n\n");
   Engine *pEngine;
   
   if(!(pEngine=engOpen(NULL)))
	{
		printf("Can not open the MATLAB engine.\n");
	}

   if(rain_s/cnt==0)
   {
	   engEvalString(pEngine,"img=imread('C:\Users\dltjr\Documents\Visual Studio 2012\Projects\sw_basic_weather\sw_basic_weather\sun.jpg')");
	   engEvalString(pEngine,"figure(1),suptitle('맑음')");
	   engEvalString(pEngine,"imshow(img,'border','tight')");
   }
   else
   {
	   engEvalString(pEngine,"img=imread('C:\Users\dltjr\Documents\Visual Studio 2012\Projects\sw_basic_weather\sw_basic_weather\rain.jpg')");
	   engEvalString(pEngine,"figure(1),suptitle('비')");
	   engEvalString(pEngine,"imshow(img,'border','tight')");
   }

   /*FILE *wfile = fopen("인천.txt","a");
   fprintf(wfile,"\n인천	%d	 %02d	 %02d	%03d	%d",input_date,l_tem_s/cnt,h_tem_s/cnt,rain_s/cnt,moi_s/cnt);
   fclose(wfile);*/
   return;
}

void destroy_save_dll()
{
   if (save_head == 0)
   {
      return;
   }
   while (save_head->next != 0)
   {
      struct save* temp = save_head;
      save_head = save_head->next;
      save_head->prev = 0;
      free(temp);
   }
   struct save* temp_2 = save_head;
   free(temp_2);
   save_head = 0;
   save_tail = 0;
   //free(save_head);
}
void destroy_data_dll()
{
   if (head == 0)
   {
      return;
   }
   while (head->under != 0)
   {
      struct node* temp = head;
      head = head->under;
      head->up = 0;
      free(temp);
   }
   struct node* temp_2 = head;
   free(temp_2);
   head = 0;
   tail = 0;
}
void destroy_find_dll()
{
   if (find_head == 0)
   {
      printf("no list\n");
      return;
   }
   while (find_head->under != 0)
   {
      struct node* temp = find_head;
      find_head = find_head->under;
      find_head->up = 0;
      free(temp);
   }
   struct node* temp_2 = find_head;
   free(temp_2);
   find_head = 0;
   find_tail = 0;
}

void destroy_data()
{
   destroy_save_dll();
   destroy_data_dll();
   destroy_find_dll();
   
   return;
}
// 최소자승법을 참고하여 습도 정리
int find_moi()
{
   // 1, 찾고자 하는 날짜 입력
   //int find_date = 0;
   //printf("찾고자 하는 날짜 입력: ");
   //scanf_s("%d", &find_date); printf("%d",find_date);

   FILE* find_f = 0;
   fopen_s(&find_f, "인천.txt", "rt");

   wchar_t pst[100];
   if (find_f == 0)
   {
      printf("no file\n");
      return -1;
   }
   while (1)
   {
      fgetws(pst, 100, find_f);
      if (wcslen(pst) == 29 || wcslen(pst) == 30)
      {
         moi_parsing(pst);
      }
      else if(wcslen(pst) == 28)
      {
         moi_parsing(pst);
         break;
      }
   }
   fclose(find_f);
   
   int input_date = 0;
   printf("날짜 입력: ");
   scanf_s("%d",&input_date);
   
   struct node* f_date_node = head;
   while (1)
   {
      if (f_date_node->date == input_date)
      {

         struct node* ans = f_date_node;
         for (int ans_idx = 1; ans_idx < 7; ans_idx++)
         {
            if (ans == 0)
            {
               printf("이전 과거 데이터가 없네요....\n");
               return -1;
            }
            ans = ans->up;
         }
         find_tail = f_date_node->up;
         for (int i = 1; i < 7; i++)
         {
            f_date_node = f_date_node->up;
         }
         find_head = f_date_node;
         find_head->up->under = find_tail->under;
         find_tail->under->up = find_head->up;

         cmp_head = head;
         cmp_tail = head;

         for (int j = 1; j < 7; j++)
         {
            cmp_tail = cmp_tail->under;
         }
         break;
      }
      else if (f_date_node->under == 0)
      {
         printf("존재하지 않는 날짜 or 과거 데이터가 없습니다.\n");
         find_tail = tail;
         //1-1, 비교할 부분 find 과 찾고자 하는 부분 cmp 을 구분.
         for (int past_cnt = 1; past_cnt < 7; past_cnt++)
         {
            tail = tail->up;
         }
         find_head = tail;
         tail = tail->up;
         find_head->up = 0;
         tail->under = 0;

         //2, 비교 시작
         // 2-1, 비교할 부분 찾기
         cmp_head = head;
         cmp_tail = head;
         for (int cmp_tail_find_idx = 1; cmp_tail_find_idx < 7; cmp_tail_find_idx++)
         {
            cmp_tail = cmp_tail->under;
         }
         break;
      }
      f_date_node = f_date_node->under;
   }
   
/*   find_tail = tail;
   //1-1, 비교할 부분과 찾고자 하는 부분을 구분.
   for (int past_cnt = 1; past_cnt < 7; past_cnt++)
   {
      tail = tail->up;
   }
   find_head = tail;
   tail = tail->up;
   find_head->up = 0;
   tail->under = 0;

   //2, 비교 시작
   // 2-1, 비교할 부분 찾기
   cmp_head = head;
   cmp_tail = head;
   for (int cmp_tail_find_idx = 1; cmp_tail_find_idx < 7; cmp_tail_find_idx++)
   {
      cmp_tail = cmp_tail->under;
   }*/
   // 여기 까지가 처음 찾음.


   
   //습도의 오차 제곱이 가장 큰 값 찾기
    //  그 값은 -> 8869

   int moi_error_sum = 0;
   int moi_cmp_error_sum = 0;
   
   int tem_error_sum = 0;
   int tem_cmp_error_sum = 0;

   int rain_error_sum = 0;
   int rain_cmp_error_sum = 0;

   struct node* s_cmp_head = cmp_head;
   struct node* s_cmp_tail = cmp_tail;
   struct node* s_find_head = find_head;

   while(s_cmp_tail != 0)
   {
      moi_error_sum = 0;
      struct node* cmp_node = s_cmp_head;
      struct node* find_node = s_find_head;
      struct node* cmp_tail = s_cmp_tail;

      while (find_node != find_tail)
      {
         moi_error_sum = moi_error_sum + (find_node->moi - cmp_node->moi)*(find_node->moi - cmp_node->moi);
         tem_error_sum = tem_error_sum + (find_node->high_tem - cmp_node->high_tem)*(find_node->high_tem - cmp_node->high_tem);
         rain_error_sum = rain_error_sum + (find_node->rain - cmp_node->rain)*(find_node->rain - cmp_node->rain);

         cmp_node = cmp_node->under;
         find_node = find_node->under;
      }
      if (moi_cmp_error_sum == 0)
      {
         moi_cmp_error_sum = moi_error_sum;
      }
      else if (tem_cmp_error_sum == 0)
      {
         tem_cmp_error_sum = tem_error_sum;
      }
      else if (rain_cmp_error_sum == 0)
      {
         rain_cmp_error_sum = rain_error_sum;
      }
      else if (moi_cmp_error_sum < moi_error_sum)
      {
         moi_cmp_error_sum = moi_error_sum;
      }
      else if (tem_cmp_error_sum < tem_error_sum)
      {
         tem_cmp_error_sum = tem_error_sum;
      }
      else if (rain_cmp_error_sum < rain_error_sum)
      {
         rain_cmp_error_sum = rain_error_sum;
      }
      else
      {
         printf("가장 큰 값 찾기 error\n");
      }
      s_cmp_head = s_cmp_head->under;
      s_cmp_tail = s_cmp_tail->under;
   }



   //printf("%d",moi_cmp_error_sum);
   

   // 확률로써 filtering 오차율은 100 - (error_sum/가장 큰 값(10143)) * 100  습도: 10143 최온; 4535 강수: 65014
   

   int sad_cnt = 0;

   //습도 filter

   //습도 일치율 기준, 습도 가장 큰 값 10143 // 90% <887 95% <507 97% 304
   int moi_percent = 90;
   int moi_percent_plus = (100 - moi_percent) * moi_cmp_error_sum / 100;

   while (cmp_tail != 0)
   {
      int error_sum = 0;  
      struct node* cmp_node = cmp_head;
      struct node* find_node = find_head;

      while (cmp_node != cmp_tail->under)
      {
         error_sum = error_sum + (cmp_node->moi - find_node->moi)*(cmp_node->moi - find_node->moi);
         cmp_node = cmp_node->under;
      }
     
      if (error_sum < moi_percent_plus)
      {
         if (cmp_tail->under == 0)
         {
            break;
         }
         //printf("cmp_head: %d , 일치율: %d\n", cmp_head->date, 100 - (error_sum / 234798) * 100);
         add_save_dll(cmp_head);
         sad_cnt++;
         //printf("date: %d  moi; %d ", cmp_head->date,cmp_head->moi);
         //printf("%d\n",sad_cnt);
      }
      cmp_head = cmp_head->under;
      cmp_tail = cmp_tail->under;
   }
   
   //3, 기온 filter
   struct save* save_cmp_flag = save_head;
   struct node* find_node = find_head;
   struct node* tem_cmp_flag = save_cmp_flag->node_posi;
   //int cnt = 0;

   int tem_percent = 99;
   int tem_percent_plus = (100 - tem_percent) * tem_cmp_error_sum / 100;

   while (1)
   {
      if (save_cmp_flag == 0)
      {
         break;
      }
      //moi_cmp_flag_tail 찾기
      int error_sum = 0;
      tem_cmp_flag = save_cmp_flag->node_posi;
      for (int moi_cmp_flag_idx = 0; moi_cmp_flag_idx < 6; moi_cmp_flag_idx++) 
      {
         tem_cmp_flag = tem_cmp_flag->under;
      }
      struct node* moi_cmp_flag_tail = tem_cmp_flag;
      tem_cmp_flag = save_cmp_flag->node_posi;

      //show_save();
      //조건에 맞춰 맞추어 보고 조건에 일치 하지 않는 것을 필터링
      while (tem_cmp_flag != moi_cmp_flag_tail->under)
      {
         error_sum = error_sum + (tem_cmp_flag->high_tem - find_node->high_tem)*(tem_cmp_flag->high_tem - find_node->high_tem);
         tem_cmp_flag = tem_cmp_flag->under;
      }
      //printf("%d  error_sum: %d\n", save_cmp_flag->node_posi->date, error_sum);
   
      if (error_sum > tem_percent_plus) // 90% 454 95% 226
      {
         
         if (moi_cmp_flag_tail->under == 0)
         {
            break;
         }
         //cnt++;
         //printf("%d\n",cnt);
         
         if (save_cmp_flag->next == 0)  // 마지막을 삭제할때 next가 없기 때문에.
         {
            if (save_cmp_flag->prev != 0)
            {
               save_cmp_flag->prev->next = 0;
               free(save_cmp_flag);
               break;
            }
            else
            {
               free(save_cmp_flag);
               break;
            }
         }
         save_cmp_flag = save_cmp_flag->next;
         delet_save_dll(save_cmp_flag->prev);
         continue;
      }

      save_cmp_flag = save_cmp_flag->next;
   }

   //강수 필터링
   save_cmp_flag = save_head;
   find_node = find_head;
   struct node* rain_cmp_flag = save_cmp_flag->node_posi;
   //int cnt = 0;

   int rain_percent = 99;
   int rain_percent_plus = (100 - rain_percent) * rain_cmp_error_sum / 100;

   while (1)
   {
      if (save_cmp_flag == 0)
      {
         break;
      }
      //rain_cmp_flag_tail 찾기
      int error_sum = 0;
      rain_cmp_flag = save_cmp_flag->node_posi;
         for (int rain_cmp_flag_idx = 0; rain_cmp_flag_idx < 6; rain_cmp_flag_idx++)
         {
            rain_cmp_flag = rain_cmp_flag->under;
         }
      struct node* rain_cmp_flag_tail = rain_cmp_flag;
      rain_cmp_flag = save_cmp_flag->node_posi;

      //조건에 맞춰 맞추어 보고 조건에 일치 하지 않는 것을 필터링
      while (rain_cmp_flag != rain_cmp_flag_tail->under)
      {
         error_sum = error_sum + (rain_cmp_flag->rain - find_node->rain)*(rain_cmp_flag->rain - find_node->rain);
         rain_cmp_flag = rain_cmp_flag->under;
      }
      //printf("date: %d sum: %d\n",rain_cmp_flag->date,error_sum);
      if (error_sum > rain_percent_plus) //가장 큰 값 65014
      {
         if (rain_cmp_flag_tail->under == 0)
         {
            break;
         }
         //cnt++;
         //printf("%d\n", cnt);
         save_cmp_flag = save_cmp_flag->next;
         if (save_cmp_flag == 0)  // 마지막을 삭제할때 next가 없기 때문에.
         {
            break;
         }
         delet_save_dll(save_cmp_flag->prev);
         continue;
      }

      save_cmp_flag = save_cmp_flag->next;
   }

   show_save(input_date);

   return 0;
}

int main()
{

   find_moi();
   //show_save();
   //destroy_data();
   
}