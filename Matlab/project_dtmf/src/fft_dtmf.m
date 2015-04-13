s = input('n-'); % 输入数字个数
l = input('n0-');
out = zeros(1600*s+l, 1);
tab = [941 1336; 697 1209; 697 1336; 697 1477; 770 1209;
       770 1336; 770 1477; 852 1209; 852 1336; 852 1477];%建立表矩阵
for i = 1:s
	k = input('0-9-');% 输入具体数字
	f1 = tab(k+1,1)/8000; %对信号取样
	f2 = tab(k+1, 2)/8000;
	for j = 1:800
		z = sin(f1*j*2*pi) + sin(f2*j*2*pi);
		out(1600*(i-1)+j+l, 1) = z;
		out(1600*(i-1)+j+800+l, 1) = 0;
	end
end

out = out./2;
subplot(211);plot(out);

n = 256;
r = zeros(8*s, n/4);
a = out;
subplot(212);plot(a);

for i = 1:8*s
	x = a((i-1)*200+1 : i*200);
	y = fft(x, n); %fft analysis
	p = abs(y); % caculate frequency
	r(i,:) = p(1:n/4)';
end

for i = 1:8*s %decode by FFT's result
	c = r(i,:);
	z = find(c<40);
	c(z) = zeros(size(z));
	b = nnz(c);
	if (b==2)
		q = find(c);
		if (q(1) == 23)
			if (q(2) == 40)
				res(i) = 1;
			elseif(q(2) == 44)
				res(i) = 2;
			elseif(q(2) == 48)
				res(i) = 3;
			end
		elseif(q(1) == 26)
			if (q(2) == 40)
				res(i) = 4;
			elseif(q(2) == 44)
				res(i) = 5;
			elseif(q(2) == 48)
				res(i) = 6;
			end
		elseif(q(1) == 28)
			if (q(2) == 40)
				res(i) = 7;
			elseif(q(2) == 44)
				res(i) = 8;
			elseif(q(2) == 48)
				res(i) = 9;
			end
		elseif(q(1) == 31)
			if (q(2) == 44)
				res(i) = 0;
			end
		end
	else
		res(i) = NaN;
	end
end

res = res
