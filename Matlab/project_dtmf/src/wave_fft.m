%”Ô“Ù–≈∫≈
[x,FS,bits]=wavread('C:\usr\root\temp\Matlab\project_dtmf\dtmf_training_files\Button1_1.wav');
%[x,FS,bits]=wavread('C:\usr\root\temp\Matlab\dtmf.wav');
x=x(:,1); 
figure(1); 
subplot(211);
plot(x); 
%sound(x,FS,bits); 
title('”Ô“Ù–≈∫≈ ±”Ú≤®–ŒÕº') 
y=fft(x,1000); 
f=(FS/1000)*[1:1000]; 
subplot(212); 
plot(f(1:300),abs(y(1:300))); 
title('”Ô“Ù–≈∫≈∆µ∆◊Õº');
