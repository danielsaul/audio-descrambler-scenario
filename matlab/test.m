[scr_y,fs] = audioread('scrambled.wav');
N = length(scr_y);

figure(1);
t = 1/fs:1/fs:N/fs;
plot(t,scr_y);  % Scrambled time domain

figure(2);
f = linspace(-fs/2,fs/2,N);
scr_Y = fft(scr_y,N);
plot(f,fftshift(abs(scr_Y))); % Scrambled freq domain

[ori_y,fs] = audioread('original.wav');
N = length(ori_y);

figure(3);
t = 1/fs:1/fs:N/fs;
plot(t,ori_y); % Original time domain

figure(4);
f = linspace(-fs/2,fs/2,N);
ori_Y = fft(ori_y,N);
plot(f,fftshift(abs(ori_Y))); % Original freq domain

figure(5);
diff_y = scr_y - ori_y;
plot(t,diff_y);

figure(6);
diff_Y = scr_Y - ori_Y;
plot(f,fftshift(abs(diff_Y)));

