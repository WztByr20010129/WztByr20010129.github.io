#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define DATASTART 16384
#define DATAEND 32768 
/*小心问题:寄存器5,寄存器6指向的内存的数据相加,数据保存到寄存器5中,那么内存数据域中的数需不需要修改???*/ 
typedef struct axptr{
	int data;//自身的数据 
	short ptr;//指向的内存数据暂存值 
	int memptr;//指向内存开始数据的指针 
} AX;
//从文件中读取指令 
int readDIR(FILE *fp,char Memory[],int size); 
//分析指令 
int divideInstruct(char buffer[],int size,char dir[]); 
//二进制转十进制 
long long getTenth(char binary[],int size); 
//取反 
int getNegation(char binary[],int size);
//十进制转二进制 
int getBinary(int n);
//打印代码段内存 
void printDirect(char dir[][35],int size);
//从字符串中截取所需要的部分 
void subStringByPos(char buffer[],char *p,int start,int end,int pStart); 
//分析指令的功能 
int anlaysis(char dir[]);
//判断到底是地址寄存器还是数据寄存器; 
int isPtrOrData(int n);
//把东西放置在内存中 ,最后应该返回最终的内存地址 
int PutInMemory(char Memory[],char beput[],int dataleft);
//分析dest是什么类型 
void* getDest(int axdest,int *dataAx[],AX* axptrs[],int number);
//分析from是什么类型 
void* getFrom(int axdest,int *dataAx[],AX* axptrs[],int number); 
//每次打印所有的信息
void printInfo(int *dataAx[],AX* axptrs[],int ir,int flag,int ip);
//封装一个函数,这个函数把地址寄存器中的数写入内存
int writeAs(char Memory[],short data,int dataleft,char ModifyShort[]); 
//封装一个函数,这个函数打印数据域
void printDataArea(char Memory[],int start); 
//封装一个函数,观察地址域内存的变化情况
void testAx(AX *axptrs[]); 
void d_10(char dir[],int *i,int *aptr,int *ip,short numbers[]);//d_10
void jump_f(int *i,int *aptr,int *ip,int dir_num,int jump_num);//jump_f
//选择对应的处理方案并处理---->是CPU的核心,返回值是dataleft,就是数据域的下一个起始点 
int switchMethods(int *dataAx[],AX* axptrs[],int d,char ModifyShort[],char dir[],int dataleft);  
char Memory[35*1024];
int flag; 
//主函数开始 
int main(void){
	/*第一阶段内容:顺利读取指令+获取指
	令的立即数和指令本身*/ 
	FILE *fp=NULL;
	char buffer[512][35];memset(Memory,0,sizeof(Memory));memset(buffer,0,sizeof(buffer));
	
	int dirsize=readDIR(fp,Memory,35*1024);	//读取文件 
	
	char dir[dirsize][33];short numbers[dirsize]={0}; //构建指令数组 //构建立即数的数组 	
	memset(dir,0,sizeof(dir));
 
	int i=0;//临时变量 
	int j=0;//临时变量 
	
	/*第一阶段结束*/ 
	
	/*第二阶段内容:构建变量来保证指令的正常执行*/
	int ip=0;flag=0;int ir=0;//指令寄存器
	int ax1=0;int ax2=0;int ax3=0;int ax4=0;//数据寄存器 
	AX ax5;ax5.data=0;ax5.ptr=NULL;AX ax6;ax6.data=0;ax6.ptr=NULL;AX ax7;ax7.data=0;ax7.ptr=NULL;AX ax8;ax8.data=0;ax8.ptr=NULL;//地址寄存器
	ax5.memptr=-1;ax6.memptr=-1;ax7.memptr=-1;ax8.memptr=-1;//内存地址的指向位置,初始值代表没有任何的指向问题,可能逻辑和地址寄存器是否指向了内存有关 
	int* dataAx[4]={&ax1,&ax2,&ax3,&ax4};AX* axptrs[4]={&ax5,&ax6,&ax7,&ax8};//创建两个指针数组,方便之后直接改动内容 
	char ModifyShort[17];memset(ModifyShort,'0',sizeof(ModifyShort));//模拟内存中的数据域,小心 因为可能数据域中有负数出现 
	int dataleft,dataright;dataleft=DATASTART;dataright=dataleft+16;
	int direction,aptr;//接收传回的指令信息	//printf("dirsize=%d\n",dirsize);
	
	/*第二阶段:变量定义部分结束*/	 
	
	/*第三阶段:核心逻辑处理开始*/ 
	
	for(aptr=0;i<=dirsize;aptr++,j++){
		buffer[i][j]=Memory[aptr];
		if(Memory[aptr]=='\n'){
			numbers[i]=divideInstruct(buffer[i],dirsize,dir[i]);//这里返回一个立即数(指令的后16位,在numbers[i]中,dir[i]中是指令前16位,buffer[i]是全部  
			sprintf(ModifyShort,"%d",numbers[i]);//模拟short类型,利用左右指针固定short类型的位置,然后递增16为单位 //printf("%s\n",ModifyShort)
			direction=anlaysis(dir[i]);//分析指令是哪一个printf("direction:%d\n",direction);
//			printf("direction:%d,aptr=%d\n",direction,aptr);
			ir=getTenth(dir[i],16);//求ir转为10进制的结果,这里的ir必须在i之前读取,否则会有问题 
//			printf("当前指令的ir值:%d",ir);printf("执行第几条指令:%d,指令为:%s\n",i,dir[i]);
			if(direction==10){//跳转的时候对寄存器内的数没有影响 
				d_10(dir[i],&i,&aptr,&ip,numbers); 
			}
			if(direction!=10){//不跳转的时候 
//				printf("aptr:normal:%d\n",aptr);
				dataleft=switchMethods(dataAx,axptrs,direction,ModifyShort,dir[i],dataleft);//返回一个dataleft值printf("flag=%d\n",flag);
				ip+=4;//ip的自增
				i++;//这里会影响numbers[i]的值,由于i++后,没有可选到的值,而numbers[i]中的值被初始化为0,故必然有问题,numbers[i]恒为0  
//				printf("dataleft=%d,ip=%d,direction=%d\n",dataleft,ip,direction); 	
			}
			j=-1;
//			printf("当前函数ip值:%d\n",ip); 
			//测试:打印一下这个dataInfo 
			printInfo(dataAx,axptrs,ir,flag,ip);
			if(dataleft==-1){
				break;
			}
//			testAx(axptrs);
		}
	}
	
	/*第三阶段:核心处理部分代码结束*/
	
	/*第四阶段:打印内存段和代码段开始*/ 
	
	printDirect(buffer,128);//打印代码段内存 
	printDataArea(Memory,DATASTART);//打印数据段内存
	
	/*第四阶段结束*/ 
	return 0;
}
void jump_f(int *i,int *aptr,int *ip,int dir_num,int jump_num){
	*i=*i+(dir_num);//i=14; 
	if(*i!=0){
		*aptr=33*(*i)-1;//数学规律 
	}
	else if(*i==0){
		*aptr=0;	
	}
	*ip+=jump_num;
}
void d_10(char dir[],int *i,int *aptr,int *ip,short numbers[]){
	char jump[5]={'\0'};
	subStringByPos(dir,jump,12,16,0);
	int jump_flag=getTenth(jump,5);//是怎么进行跳转的 
	int jump_num=numbers[*i];//跳转的步数,往回跳转-28,但是我这个指令自己已经读完了,下一次执行的是-28+4=-24的位置的指令 
	int dir_num=jump_num/4;//printf("numbers[i]=%d,i=%d\njump_num:%d\ndirnum:%d\n",numbers[i],i,jump_num,dir_num);//打印
	if(jump_flag==0){//无条件跳转 
		jump_f(i,aptr,ip,dir_num,jump_num);  
//		printf("无条件跳转:跳转后:i:%d,ip:%d,*aptr:%d\n",i,ip,*aptr);//无条件跳转 
	}
	if(jump_flag==1&&flag==0){//flag为0跳转 
		jump_f(i,aptr,ip,dir_num,jump_num);  			
	}
	else if(jump_flag==1&&flag!=0){
		*ip+=4;
		*i=*i+1;
	}
	if(jump_flag==2&&flag==1){//flag为1跳转 
		jump_f(i,aptr,ip,dir_num,jump_num); 
		//printf("i=%d,flag为1跳转,Memory:%c\n",i,Memory[*aptr]);//这里可以知道Memory[*aptr]是什么呢?是一个换行符 
	}
	else if(jump_flag==2&&flag!=1){
		*ip+=4;
		*i=*i+1;
	} 
	if(jump_flag==3&&flag==-1){//flag为-1跳转 
		jump_f(i,aptr,ip,dir_num,jump_num); 	
	}
	else if(jump_flag==3&&flag!=-1){
		*ip+=4;
		*i=*i+1;
	}  		
}
void testAx(AX *axptrs[]){//观察地址寄存器的变化情况 
	//赋值操作 
	printf("\n");
	for(int i=0;i<4;i++){
		printf("第%d个,内存地址:%d,指向的值:%d\t",i,axptrs[i]->memptr,axptrs[i]->ptr);
	}
	printf("\n");
}
void subStringByPos(char buffer[],char p[],int start,int end,int pStart){//函数:截取字符串的前n位并在容器中存储,左闭右开 
	for(int i=start;i<end;i++){
		p[pStart++]=buffer[i];
	}
	p[pStart]='\0';//补上字符串的结束符 
}
int isPtrOrData(int n){
	if(n>0&&n<=4){
		return 0;//数据寄存器 
	}else if(n>=5&&n<8){
		return 1;//地址寄存器 
	}else if(n==0){
		return -1;//立即数 
	}
	printf("isptrOrdata:Error 原因是在switch_method中 无法获得内存地址,n=%d\n",n); 
	return -2; 
}
void *getFrom(int axfrom,int *dataAx[],AX* axptrs[],int number){
	int ffrom=isPtrOrData(axfrom);
	if(ffrom==0){
		return dataAx[axfrom-1];
	}else if(ffrom==1){
		return axptrs[axfrom-5];
	}else if(ffrom==-1){
		return &number;
	}
}
void* getDest(int axdest,int *dataAx[],AX* axptrs[],int number){
	int fdest=isPtrOrData(axdest);
	if(fdest==0){
		return dataAx[axdest-1];
	}else if(fdest==1){
		return axptrs[axdest-5];
	}else if(fdest==-1){
		return &number;
	}
}
int switchMethods(int *dataAx[],AX* axptrs[],int d,char ModifyShort[],char dir[],int dataleft){//去处理 
	char dest[5]={'\0'};char from[5]={'\0'}; 
	subStringByPos(dir,dest,8,12,0);subStringByPos(dir,from,12,16,0);//解决问题:1.往哪里移动 2.移动到哪里
	int axdest=getTenth(dest,5);int axfrom=getTenth(from,5);int number=atoi(ModifyShort);//由地址获取index的值,该转换为int的都转换为int 
	int fdest=isPtrOrData(axdest);int ffrom=isPtrOrData(axfrom);//构造fdest,ffrom,flag 
//	printf("axdest=%d,axfrom=%d,fdest=%d,ffrom=%d\n",axdest,axfrom,fdest,ffrom);
	void *dest_final=getDest(axdest,dataAx,axptrs,number);//获取dest是哪一个地址寄存器,哪一个数据寄存器 
	void *from_final=getFrom(axfrom,dataAx,axptrs,number);//获取from是哪一个地址寄存器,哪一个数据寄存器 	
	//以上解决了寄存器的问题 
	int curPtr=dataleft;//返回的数据,这里解决了内存部分到底返回什么的问题 
	if(d==0){
		curPtr=-1;
	}
	else if(d==1){//传送
//		printf("值:%d\n",*(int *)from_final);
		if(ffrom==-1){//把立即数传入数据寄存器 
			if(fdest==0){
//				*(dataAx[axdest-1])=number; 
				*(int *)dest_final=*(int *)from_final;
			}else if(fdest==1){
				((AX *)dest_final)->data=*(int *)from_final;
//				axptrs[axdest-5]->data=number;//结构体指针
//				printf("地址:number:%d\n",number);	
			}
		}else{//地址寄存器和数据寄存器的互相转换 
			if(fdest==0&&ffrom==0){//数据寄存器 
//				*dataAx[axdest-1]=*dataAx[axfrom-1];
				*(int *)dest_final=*(int *)from_final;
			}
			else if(fdest==0&&ffrom==1){//地址寄存器中的值传入数据寄存器中 
//				short my_data=axptrs[axfrom-5]->ptr;
//				*dataAx[axdest-1]=(int)my_data;
				short my_data=((AX *)from_final)->ptr;
				*(int *)dest_final=(int)my_data;
//				printf("这里是data=%d,sizeof(%d)\n",*dataAx[axdest-1],sizeof(*dataAx[axdest-1]));
			}
			else if(fdest==1&&ffrom==1){ 
				((AX *)dest_final)->ptr=((AX *)from_final)->ptr;//指向的值 
				((AX *)dest_final)->memptr=((AX *)from_final)->memptr;//指向的地址也改变 
//				axptrs[axdest-5]->ptr=axptrs[axfrom-5]->ptr;
//				printf("axdest=%d\n",axptrs[axdest-5]->ptr);
			}else if(fdest==1&&ffrom==0){//数据寄存器中的值传入地址寄存器中 ,地址寄存器中可能是有数的!,这个时候就不是写入内存了,而是修改内存 
//				int data=*dataAx[axfrom-1];	//数据域中的值没有改变
//				int bin=getBinary(data);sprintf(ModifyShort,"%d",bin);//data要存放到内存中
//				curPtr=PutInMemory(Memory,ModifyShort);//获取返回的内存指针,填充到内存中
//				axptrs[axdest-5]->ptr=data;//地址寄存器获取值
				int data=*(int *)from_final;
				((AX *)dest_final)->ptr=data;
//				printf("数据域转到地址域,data:%d\n",data);
				if(((AX *)dest_final)->memptr==-1){//判断内存中的值是添加还是修改 
					curPtr=writeAs(Memory,data,dataleft,ModifyShort);//curPtr是内存地址的下一个起始位
					((AX *)dest_final)->memptr=curPtr-16;//内存所指向的值的开头赋给右侧 	
				}else {
					writeAs(Memory,data,((AX *)dest_final)->memptr,ModifyShort);
//					curPtr=curPtr-16;
				}			
//				printf("data=%d,bin=%d,ModifyShort:%s\n",data,bin,ModifyShort);
			}
		}
	}
	else if(d==2){
		if(ffrom==-1){//立即数 
			if(fdest==0){ 
				*(int *)dest_final+=number;
			}
			else if(fdest==1){
				((AX *)dest_final)->data+=number;//data和立即数相加,而不是其他的和立即数相加 
				short my_data=((AX *)dest_final)->data; 
				writeAs(Memory,my_data,((AX *)dest_final)->memptr,ModifyShort);//重写地址域所指向的内存区域的值 
			}
		} 
		if(fdest==0&&ffrom==0){//数据域 
			*(int *)dest_final=*(int *)dest_final+*(int *)from_final;
		}
		if(fdest==1&&ffrom==1){//地址域 
			short data=((AX *)from_final)->ptr;
			((AX *)dest_final)->ptr+=data;
			//修改地址域中指向内存的数,而不是添加 
			writeAs(Memory,data,((AX *)dest_final)->memptr,ModifyShort);//重写地址域所指向的内存区域的值
		}
		if(fdest==0&&ffrom==1){
			*(int *)dest_final+=((AX *)from_final)->ptr;
		}
		if(fdest==1&&ffrom==0){
			((AX *)dest_final)->ptr+=*(int *)from_final;
			//修改地址域中指向内存的数,而不是添加 
			writeAs(Memory,((AX *)dest_final)->ptr,((AX *)dest_final)->memptr,ModifyShort);//重写地址域所指向的内存区域的值
		}
	}
	else if(d==3){
		if(ffrom==-1){//立即数 
			if(fdest==0){ 
				*(int *)dest_final-=number;
			}
			else if(fdest==1){
				((AX *)dest_final)->data-=number;//data和立即数相加,而不是其他的和立即数相加 
				short my_data=((AX *)dest_final)->data; 
				writeAs(Memory,my_data,((AX *)dest_final)->memptr,ModifyShort);//重写地址域所指向的内存区域的值 
			}
		} 
		if(fdest==0&&ffrom==0){//数据域 
			*(int *)dest_final=*(int *)dest_final-*(int *)from_final;
		}
		if(fdest==1&&ffrom==1){//地址域 
			short data=((AX *)from_final)->ptr;
			((AX *)dest_final)->ptr-=data;
			//修改地址域中指向内存的数,而不是添加 
			writeAs(Memory,data,((AX *)dest_final)->memptr,ModifyShort);//重写地址域所指向的内存区域的值
		}
		if(fdest==0&&ffrom==1){
			*(int *)dest_final-=((AX *)from_final)->ptr;
		}
		if(fdest==1&&ffrom==0){
			((AX *)dest_final)->ptr-=*(int *)from_final;
			//修改地址域中指向内存的数,而不是添加 
			writeAs(Memory,((AX *)dest_final)->ptr,((AX *)dest_final)->memptr,ModifyShort);//重写地址域所指向的内存区域的值
		}
	}
	else if(d==4){
			if(ffrom==-1){//立即数 
			if(fdest==0){ 
				*(int *)dest_final*=number;
			}
			else if(fdest==1){
				((AX *)dest_final)->data*=number;//data和立即数相加,而不是其他的和立即数相加 
				short my_data=((AX *)dest_final)->data; 
				writeAs(Memory,my_data,((AX *)dest_final)->memptr,ModifyShort);//重写地址域所指向的内存区域的值 
			}
		} 
		if(fdest==0&&ffrom==0){//数据域 
			*(int *)dest_final=*(int *)dest_final*(*(int *)from_final);
		}
		if(fdest==1&&ffrom==1){//地址域 
			short data=((AX *)from_final)->ptr;
			((AX *)dest_final)->ptr*=data;
			//修改地址域中指向内存的数,而不是添加 
			writeAs(Memory,data,((AX *)dest_final)->memptr,ModifyShort);//重写地址域所指向的内存区域的值
		}
		if(fdest==0&&ffrom==1){
			*(int *)dest_final*=((AX *)from_final)->ptr;
		}
		if(fdest==1&&ffrom==0){
			((AX *)dest_final)->ptr*=*(int *)from_final;
			//修改地址域中指向内存的数,而不是添加 
			writeAs(Memory,((AX *)dest_final)->ptr,((AX *)dest_final)->memptr,ModifyShort);//重写地址域所指向的内存区域的值
		}
	}
	else if(d==5){
			if(ffrom==-1){//立即数 
			if(fdest==0){ 
				*(int *)dest_final/=number;
			}
			else if(fdest==1){
				((AX *)dest_final)->data/=number;//data和立即数相加,而不是其他的和立即数相加 
				short my_data=((AX *)dest_final)->data; 
				writeAs(Memory,my_data,((AX *)dest_final)->memptr,ModifyShort);//重写地址域所指向的内存区域的值 
			}
		} 
		if(fdest==0&&ffrom==0){//数据域 
			*(int *)dest_final=*(int *)dest_final/(*(int *)from_final);
		}
		if(fdest==1&&ffrom==1){//地址域 
			short data=((AX *)from_final)->ptr;
			((AX *)dest_final)->ptr/=data;
			//修改地址域中指向内存的数,而不是添加 
			writeAs(Memory,data,((AX *)dest_final)->memptr,ModifyShort);//重写地址域所指向的内存区域的值
		}
		if(fdest==0&&ffrom==1){
			*(int *)dest_final/=((AX *)from_final)->ptr;
		}
		if(fdest==1&&ffrom==0){
			((AX *)dest_final)->ptr/=*(int *)from_final;
			//修改地址域中指向内存的数,而不是添加 
			writeAs(Memory,((AX *)dest_final)->ptr,((AX *)dest_final)->memptr,ModifyShort);//重写地址域所指向的内存区域的值
		}
	}
	else if(d==6){//逻辑与 
		if(ffrom==-1){
			int fromdata=*(int *)from_final;
			if(fdest==0){
				int destdata=*(int *)dest_final;
				*(int *)dest_final=fromdata&&destdata;
			}
			if(fdest==1){//修改内存
				int destdata=(int)((AX*)dest_final)->ptr;
				((AX*)dest_final)->ptr=fromdata&&destdata;//修改值 
				writeAs(Memory,fromdata&&destdata,((AX*)dest_final)->memptr,ModifyShort);//修改内存			
			}	
		}
		if(ffrom==0&&fdest==0){
			*(int *)dest_final=(*(int *)dest_final)&&(*(int *)from_final);//修改值 
		}
		if(ffrom==1&&fdest==0){
			*(int *)dest_final=((AX *)from_final)->ptr&&(*(int *)dest_final);//修改值 
		}
		if(ffrom==0&&fdest==1){
			int datafrom=*(int *)from_final;
			int datadest=((AX*)dest_final)->ptr;
			((AX*)dest_final)->ptr=datafrom&&datadest;
			writeAs(Memory,((AX*)dest_final)->ptr,((AX*)dest_final)->memptr,ModifyShort);
		}
		if(ffrom==1&&fdest==1){
			int data1=((AX*)dest_final)->ptr;
			int data2=((AX*)from_final)->ptr;
			int dataflag=data1&&data2;
			((AX*)dest_final)->ptr=dataflag;
			writeAs(Memory,dataflag,((AX*)dest_final)->memptr,ModifyShort);//修改内存
		}
	}
	else if(d==7){//逻辑或 
		if(ffrom==-1){
			int fromdata=*(int *)from_final;
			if(fdest==0){
				int destdata=*(int *)dest_final;
				*(int *)dest_final=fromdata||destdata;
			}
			if(fdest==1){//修改内存
				int destdata=(int)((AX*)dest_final)->ptr;
				((AX*)dest_final)->ptr=fromdata||destdata;//修改值 
				writeAs(Memory,fromdata||destdata,((AX*)dest_final)->memptr,ModifyShort);//修改内存			
			}	
		}
		if(ffrom==0&&fdest==0){
			*(int *)dest_final=(*(int *)dest_final)||(*(int *)from_final);//修改值 
		}
		if(ffrom==1&&fdest==0){
			*(int *)dest_final=((AX *)from_final)->ptr||(*(int *)dest_final);//修改值 
		}
		if(ffrom==0&&fdest==1){
			int datafrom=*(int *)from_final;
			int datadest=((AX*)dest_final)->ptr;
			((AX*)dest_final)->ptr=datafrom&&datadest;
			writeAs(Memory,((AX*)dest_final)->ptr,((AX*)dest_final)->memptr,ModifyShort);//修改内存	
		}
		if(ffrom==1&&fdest==1){
			int data1=((AX*)dest_final)->ptr;
			int data2=((AX*)from_final)->ptr;
			int dataflag=data1||data2;
			((AX*)dest_final)->ptr=dataflag;
			writeAs(Memory,dataflag,((AX*)dest_final)->memptr,ModifyShort);//修改内存
		} 
	}
	else if(d==8){//逻辑非 
		 if(ffrom==-1){//对某个数据寄存器内的数做逻辑非运算 
		 	*(int *)dest_final=!(*(int *)dest_final); 
		 }else if(fdest==-1){//对某个地址寄存器内的数做逻辑非运算 
		 	short data=!((AX *)from_final)->ptr;
		 	((AX *)from_final)->ptr=data;
		 	writeAs(Memory,data,((AX *)from_final)->memptr,ModifyShort);//修改内存	
		 }
	}
	else if(d==9){//比较操作
		 if(ffrom==-1){//立即数 
			int curnum=*(int *)from_final;
		 	if(fdest==0){//数据寄存器 
		 		int ddata=*(int *)dest_final;
				if(ddata<curnum){
					flag=-1;
				}else if(ddata==curnum){
					flag=0;
				}else if(ddata>curnum){
					flag=1;
				}	
			}else if(fdest==1){//地址寄存器 
				int ddata=((AX*)dest_final)->ptr;
				if(ddata<curnum){
					flag=-1;
				}else if(ddata==curnum){
					flag=0;
				}else if(ddata>curnum){
					flag=1;
				}	 
			}	
		 }else{
		 	if(ffrom==0&&fdest==0){
		 		int ddata=*(int *)dest_final;
		 		int curnum=*(int *)from_final;
				if(ddata<curnum){
					flag=-1;
				}else if(ddata==curnum){
					flag=0;
				}else if(ddata>curnum){
					flag=1;
				}	
			}
			if(ffrom==1&&fdest==0){
				int destnum=*(int *)dest_final;
				int fromnum=(int)((AX *)from_final)->ptr;
				if(destnum<fromnum){
					flag=-1;
				}
				if(destnum>fromnum){
					flag=1;
				}
				if(destnum==fromnum){
					flag=0;
				}
			}
			if(ffrom==0&&fdest==1){
				int destnum=(int)((AX *)dest_final)->ptr;
				int fromnum=*(int *)from_final;
				if(destnum<fromnum){
					flag=-1;
				}
				if(destnum>fromnum){
					flag=1;
				}
				if(destnum==fromnum){
					flag=0;
				}
			}
			if(ffrom==1&&fdest==1){
				int destnum=(int)((AX *)dest_final)->ptr;
				int fromnum=(int)((AX *)from_final)->ptr;
					if(destnum<fromnum){
					flag=-1;
				}
				if(destnum>fromnum){
					flag=1;
				}
				if(destnum==fromnum){
					flag=0;
				}
			}
		 }
	}
	else if(d==11){	//跳转操作在之前写了 
		//输入 
		int input;
		printf("in:\n");
		scanf("%d",&input);//读入了一个立即数 
		if(fdest==0){
			*(int *)dest_final=input;//存储到数据寄存器中
		}else {
			((AX *)dest_final)->ptr=(short)input;//存储到地址寄存器所指向的内存中
			if(((AX *)dest_final)->memptr==-1){
				curPtr=writeAs(Memory,((AX *)dest_final)->ptr,dataleft,ModifyShort);//这里要小心内存的覆盖情况 	
			}else{
				writeAs(Memory,((AX *)dest_final)->ptr,((AX *)dest_final)->memptr,ModifyShort);
			}
		}
	}else if(d==12){
		int outputdata;
//		printf("可以打印出值来\n"); 
		if(fdest==0){
			outputdata=*(int *)dest_final; //输出的值在数据域 
		}else if(fdest==1){
			outputdata=((AX *)dest_final)->ptr;//输出的值在地址域 
		}
		printf("out:%d\n",outputdata);
	}
	return curPtr;
} 
int writeAs(char Memory[],short data,int dataleft,char ModifyShort[]){
	int bin=getBinary(data);
//	printf("写入前的bin:%d",bin); 
	sprintf(ModifyShort,"%d",bin);
//	printf("写入前的字符串:\n"); 
//	for(int i=0;i<strlen(ModifyShort);i++){
//		printf("%c",ModifyShort[i]);
//	}
//	printf("\n");
	return PutInMemory(Memory,ModifyShort,dataleft);//写入的字符串是无误的 
}
int PutInMemory(char Memory[],char beput[],int dataleft){//返回下一次能放进来的接口,写入内存的时候,这个地方逆序了 
	int dataright;
	int putindex=strlen(beput)-1;
//	printf("beput=%s,dataleft=%d,strlen(beput)=%d\n",beput,dataleft,strlen(beput));//beput为16400 
	for(dataright=dataleft+15;dataright>dataleft;dataright--){//不取等号,因为取等以后才会出现问题,确实是只走了15位的值 
		if(putindex>=0){
			Memory[dataright]=beput[putindex];//存进去了 已确认 printf("Put:%c\n",beput[putindex-1]);
//			printf("Memory[i]:%c\n",Memory[dataright]);
			putindex--;	
		}else {
			Memory[dataright]='0';                //printf("dataright=%d,Memory:%c\n",dataright,Memory[dataright]); 
		}
	}
	Memory[dataright]='0';//第16位数 
//	printf("下面是内存段数据域转移到地址域的情况:"); 
//	for(int i=dataright;i<dataright+16;i++){
//		printf("%c",Memory[i]);
//	}
//	printf("内存,打印结束\n");
	return dataright+16;
}
int anlaysis(char dir[]){//函数:分析指令  返回值:执行的是第几条指令 
	char Whattodo[9]={'\0'};//第一步:分析指令要做什么 
	subStringByPos(dir,Whattodo,0,8,0);
//	printf("dir=%s,whattodo=%s\n",dir,Whattodo);//证明whattodo正确 
	char instructs[13][9]={"00000000","00000001","00000010","00000011","00000100","00000101","00000110","00000111","00001000","00001001",
	"00001010","00001011","00001100"};//停机 传送 加 减 乘	//除 与 或 非 比较	//跳转 输入 输出 //直接枚举二维数组是好办法 
	int temp;int index;
	for(int i=0;i<13;i++){
		temp=strcmp(Whattodo,instructs[i]);
		if(temp==0){
			index=i;
			break;
		}
	}
	return index; 
}
void printDirect(char dir[][35],int size){//函数:打印代码段的内存
	printf("代码段打印开始\n");
	for(int i=0;i<size;i++){
		printf("%lld ",getTenth(dir[i],strlen(dir[i])));
		if(i%7==0&&i!=0){
			printf("\n");
		}
	}
	printf("\n"); 
	printf("代码段打印结束\n");
} 
int readDIR(FILE *fp,char Memory[],int size){//返回这个i表示读取了几条指令 
	fp=fopen("test.txt","r");
	if(fp==NULL){
		printf("文件打不开或文件路径错误!\n"); 
		return -1;
	}
	int i;
	char ch;//文件中读取的字符 
	int index=0;
	while((ch=fgetc(fp))!=EOF){
		Memory[index]=ch;
		index++;
		if(ch=='\n'){
			i++;
		}
	}
	Memory[index]='\n';//给最后的一个字符串补上一个换行符,方便读取 
	fclose(fp);
	return i;
}
int getNegation(char binary[],int size){//取反
	char c;
	for(int i=0;i<size;i++){
		c=binary[i];
		if(c=='0'){
			binary[i]='1';
		}else if(c=='1'){
			binary[i]='0';
		}
	}
	int tenth=getTenth(binary,16);//取反加一:二进制加一 那是不是说我们要把他先转换为10进制,再转换为2进制操作
	return getBinary(tenth+1);
}
int getBinary(int n){//获取二进制数 
    int binaryNumber = 0;
    int remain, i = 1;
 
    while (n!=0){
        remain = n%2;
        n /= 2;
        binaryNumber += remain*i;
        i *= 10;
    }
    return binaryNumber;
}
long long getTenth(char binary[],int size){//获取十进制数 
	long long tenth=0;
	for(int i=0;i<size;i++){
		if(binary[i]=='\n'||binary[i]=='\0'){
			break;
		}
		tenth=tenth*2+binary[i]-'0';
	}
	return tenth;	
}
int divideInstruct(char buffer[],int size,char dir[]){//解析指令的前16位和后16位 
	//调用一个方法:2进制转换为10进制
	int i=16;
	char binary[17];
	subStringByPos(buffer,dir,0,16,0);
//	dir[16]='\0';//字符数组配上结束符才是字符串 
	subStringByPos(buffer,binary,16,32,0);
	//处理补码的问题
	int result;
	char my_bin[16];
	if(binary[0]=='0'){
		result=getTenth(binary,16);
	}else if(binary[0]=='1'){
		int bin=getNegation(binary,16);//取反加1,求其反,封装一个函数 
		sprintf(my_bin,"%d",bin);//sprintf函数把字符串转化为int类型 
		result=0-getTenth(my_bin,strlen(my_bin));
	}
//	printf("%d\n",result);
	return result;
}
void printDataArea(char Memory[],int start){//难点:对内存的操作 
	int i,data,index=0;
	char p[17];
	printf("数据域打印开始\n");
	for(i=DATASTART;i<DATASTART+4096;i+=16){
		subStringByPos(Memory,p,i,i+16,0);//printf("data:%s\n",p);
		data=getTenth(p,17);
		printf("%d ",data);index++;
		if(index%16==0){
			printf("\n");
		}
	}
	printf("数据域打印结束;\n"); 
}
void printInfo(int *dataAx[],AX* axptrs[],int ir,int flag,int ip){
	printf("\n");
	printf("ip = %d\n",ip);
	printf("flag = %d\n",flag);
	printf("ir = %d\n",ir);
	printf("ax1 = %d ax2 = %d ax3 = %d ax4 = %d\n",*dataAx[0],*dataAx[1],*dataAx[2],*dataAx[3]);
	printf("ax5 = %d ax6 = %d ax7 = %d ax8 = %d\n",axptrs[0]->data,axptrs[1]->data,axptrs[2]->data,axptrs[3]->data);
	printf("\n");
}
