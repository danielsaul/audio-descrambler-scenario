[ori_y,fs] = audioread('original.wav');
disp(fs)
N = length(ori_y);

% Original time domain
figure(1);
t = 1/fs:1/fs:N/fs;
plot(t,ori_y); 

% Original freq domain
figure(2);
f = linspace(-fs/2,fs/2,N);
ori_Y = fft(ori_y,N);
plot(f,fftshift(abs(ori_Y)));

[scr_y,fs] = audioread('scrambled.wav');
N = length(scr_y);

% Scrambled time domain
figure(3);
t = 1/fs:1/fs:N/fs;
plot(t,scr_y);

% Scrambled freq domain
figure(4);
f = linspace(-fs/2,fs/2,N);
scr_Y = fft(scr_y,N);
plot(f,fftshift(abs(scr_Y))); 
[pk,MaxFreq] = findpeaks(fftshift(abs(scr_Y)),'NPeaks',1,'SortStr','descend');
hold on
plot(f(MaxFreq),pk,'or')
hold off
Freq = f(MaxFreq)

h  = fdesign.lowpass('Fp,Fst,Ap,Ast', 7000, 7500, 1, 60, fs);
Hd = design(h, 'butter');

scr_y_lp = filter(Hd, scr_y);

% Scrambled time domain with low pass
figure(5);
t = 1/fs:1/fs:N/fs;
plot(t,scr_y_lp);

% Scrambled freq domain with low pass
figure(6);
f = linspace(-fs/2,fs/2,N);
scr_Y_lp = fftshift(fft(scr_y_lp,N));
plot(f,abs(scr_Y_lp)); 

scr_Y_lp_flip = [scr_Y_lp((N/2)+1:N), fliplr(scr_Y_lp((N/2)+1:N))];

M = length(scr_Y_lp((N/2)+1:N))
O = length(scr_Y_lp_flip)

figure(8);
f = linspace(-fs/2,fs/2,N);
plot(f,abs(scr_Y_lp_flip));

