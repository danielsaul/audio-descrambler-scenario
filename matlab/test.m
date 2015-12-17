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

% Low-pass filter
%h  = fdesign.lowpass('Fp,Fst,Ap,Ast', 7000, 7500, 1, 60, fs);
%Hd = design(h, 'butter');

% Chebyshev Band-stop filter
h  = fdesign.bandstop('N,Fp1,Fp2,Ap', 2, 7800, 8200, 1, fs);
bandstop = design(h, 'cheby1');

% Chebyshev low-pass filter
h  = fdesign.lowpass(7000, 7800, 1, 40, fs);
Hd = design(h, 'cheby1', 'MatchExactly', 'passband');

% Chebyshev low-pass 6000
h  = fdesign.lowpass('N,Fp,Ap', 4, 5000, 1, fs);
lowpass = design(h, 'cheby1');

% Remove 8kHz tone
scr_y_lp = filter(bandstop, scr_y);

% Scrambled * 7kHz
sine = sin(2*pi*7000*t).';
scr_y_sin = scr_y_lp .* sine;

% Remove upper frequencies
scr_y_sin_lp = filter(lowpass, scr_y_sin);

% Unscrambled freq domain
figure(5);
f = linspace(-fs/2,fs/2,N);
unscr_Y = fft(scr_y_sin_lp,N);
plot(f,fftshift(abs(unscr_Y)));

% Play unscrambled sound
sound(scr_y_sin_lp, fs)

