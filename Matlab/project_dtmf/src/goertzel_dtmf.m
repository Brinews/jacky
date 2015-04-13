%1. ���ò�����������8λ�绰���룻
tm=[1,2,3,65;4,5,6,66;7,8,9,67;42,0,35,68];   %DTMF�źŴ����16������
N=205;
K=[18,20,22,24,31,34,38,42];
f1=[697,770,852,941];                 %��Ƶ������      
f2=[1209,1336,1477,1633];             %��Ƶ������ 
TN=input('Input telephone number(8 digits)=');          %����8λ����
TNr=0;                                %���ն˵绰�����ֵΪ0

TOTAL=8

%2. ���������8λ�绰�������ʱ����ɢDTMF�źţ�����������8λ�����Ӧ��˫����Ƶ����
for m=1:TOTAL;
	d=fix(TN/10^(TOTAL-m));         
	TN=TN-d*10^(TOTAL-m);

	%TN

	for p=1:4;
		for q=1:4;
			if tm(p,q)==abs(d);break,end       %������mλ����������к�
		end

		if tm(p,q)==abs(d);break,end       %������mλ����������к� 
	end

	n=0:1023;                              %Ϊ�˷������ӳ�����
	x=sin(2*pi*n*f1(p)/8000)+sin(2*pi*n*f2(q)/8000);
	sound(x,8000);                          %��������  
	pause(0.1)
	%3. ��ʱ����ɢDTMF�źŽ���Ƶ�ʼ�⣬���������ף�
	X=goertzel(x(1:N),K+1);
	val=abs(X)

	X
	val

	subplot(4,2,m);stem(K,val,'.');grid;xlabel('k');ylabel('|X(k)|')
	%4.���ݷ����׵�������ֵ���ֱ���Ҳ�ȷ�������8λ�绰���룻
	limit=80;                             %��Ƶ�������Ϊ80

	for s=5:8;
		if val(s)>limit,break,end             %�����к�
	end 
	for r=1:4;
		if val(r)>limit,break,end             %�����к�
	end

	TNr=TNr+tm(r,s-4)*10^(TOTAL-m)        %��ʾ��Ϊһ��8λ��
end

disp('Receiver Detect 8 digit:')
disp(TNr)  

