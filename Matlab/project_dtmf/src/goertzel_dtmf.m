%1. 设置参数，并读入8位电话号码；
tm=[1,2,3,65;4,5,6,66;7,8,9,67;42,0,35,68];   %DTMF信号代表的16个数字
N=205;
K=[18,20,22,24,31,34,38,42];
f1=[697,770,852,941];                 %行频率向量      
f2=[1209,1336,1477,1633];             %列频率向量 
TN=input('Input telephone number(8 digits)=');          %输入8位数字
TNr=0;                                %接收端电话号码初值为0

TOTAL=8

%2. 根据输入的8位电话号码产生时域离散DTMF信号，并连续发出8位号码对应的双音多频音；
for m=1:TOTAL;
	d=fix(TN/10^(TOTAL-m));         
	TN=TN-d*10^(TOTAL-m);

	%TN

	for p=1:4;
		for q=1:4;
			if tm(p,q)==abs(d);break,end       %检测与第m位号码相符的列号
		end

		if tm(p,q)==abs(d);break,end       %检测与第m位号码相符的行号 
	end

	n=0:1023;                              %为了发声，加长序列
	x=sin(2*pi*n*f1(p)/8000)+sin(2*pi*n*f2(q)/8000);
	sound(x,8000);                          %发出声音  
	pause(0.1)
	%3. 对时域离散DTMF信号进行频率检测，画出幅度谱；
	X=goertzel(x(1:N),K+1);
	val=abs(X)

	X
	val

	subplot(4,2,m);stem(K,val,'.');grid;xlabel('k');ylabel('|X(k)|')
	%4.根据幅度谱的两个峰值，分别查找并确定输入的8位电话号码；
	limit=80;                             %基频检测门限为80

	for s=5:8;
		if val(s)>limit,break,end             %查找列号
	end 
	for r=1:4;
		if val(r)>limit,break,end             %查找行号
	end

	TNr=TNr+tm(r,s-4)*10^(TOTAL-m)        %表示成为一个8位数
end

disp('Receiver Detect 8 digit:')
disp(TNr)  

