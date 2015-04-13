clc 
clear 
fs=8000; 
flow=[697,770,852,942];fhight=[1209,1336,1477,1633]; 
telnum=input('input your number:','s'); 
lengthofnum=length(telnum); 
tone=[]; 
tones=[]; 
n=[0:399] 

for i=1:lengthofnum

	if (str2num(telnum(i))==0); 
		tone=[sin(2*pi*flow(4)*n/fs)+sin(2*pi*fhight(2)*n/fs) zeros(1,400)]; 
	elseif (str2num(telnum(i))==1); 
		tone=[sin(2*pi*flow(1)*n/fs)+sin(2*pi*fhight(1)*n/fs) zeros(1,400)]; 
	elseif (str2num(telnum(i))==2); 
		tone=[sin(2*pi*flow(1)*n/fs)+sin(2*pi*fhight(2)*n/fs) zeros(1,400)]; 
	elseif (str2num(telnum(i))==3); 
		tone=[sin(2*pi*flow(1)*n/fs)+sin(2*pi*fhight(3)*n/fs) zeros(1,400)]; 
	elseif (str2num(telnum(i))==4); 
		tone=[sin(2*pi*flow(2)*n/fs)+sin(2*pi*fhight(1)*n/fs) zeros(1,400)]; 
	elseif (str2num(telnum(i))==5); 
		tone=[sin(2*pi*flow(2)*n/fs)+sin(2*pi*fhight(2)*n/fs) zeros(1,400)]; 
	elseif (str2num(telnum(i))==6); 
		tone=[sin(2*pi*flow(2)*n/fs)+sin(2*pi*fhight(3)*n/fs) zeros(1,400)]; 
	elseif (str2num(telnum(i))==7); 
		tone=[sin(2*pi*flow(3)*n/fs)+sin(2*pi*fhight(1)*n/fs) zeros(1,400)]; 
	elseif (str2num(telnum(i))==8); 
		tone=[sin(2*pi*flow(3)*n/fs)+sin(2*pi*fhight(2)*n/fs) zeros(1,400)]; 
	elseif (str2num(telnum(i))==9); 
		tone=[sin(2*pi*flow(3)*n/fs)+sin(2*pi*fhight(3)*n/fs) zeros(1,400)]; 
	end 

	tones=[tones tone] 
end 

figure(1) 
plot(tones) 
figure(2) 
freqz(tones) 
figure(3) 
plot(abs(fft(tones))) 

wavwrite(tones,fs,16,'C:\usr\root\temp\Matlab\dtmf.wav') 
