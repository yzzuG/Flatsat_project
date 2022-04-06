%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%     Traitement des donnees - FlatSat 
%
%     GUZZONATO - SPERY
%
%      04/04/2022
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear all
close all
clc

%package pour ouvrir des documents textes sur octave
pkg load io;


%% recuperation des donnees

data_prev = load('Wifi_record.txt');

[taille colonne]=size(data_prev);
der_data = data_prev(taille,1);

num_data = 1:1:der_data;

%% Trie des infos
data_temp = sortrows(data_prev,1);

for i = [1:der_data]
  
  col = find(data_temp(:,1)==i);

  if (col>0)
    data (i,:)= data_temp(col,:);
  endif
 
endfor

data(:,1)=num_data;


EPS = data(:,2:5);
Temp = data(:,6);
altitude = data(:,7);
pression = data(:,8);
Euler = data(:,9:11);
Quaternion = data(:,12:15);
v_angulaire = data(:,16:18);
accel = data(:,19:21);
chp_magn = data(:,22:24);
gravite = data(:,25:27);
luminance = data(:,28:32);
format long;
GNSS = data(:,33:35);

[rows columns] = size(data);


if columns > 35
  Rssi = data(:,36);
end

figure
subplot(3,1,1)
plot(num_data,EPS(:,1))
title('EPS')
xlabel('Numero de trame')
ylabel('Vbat (V)')
grid on
subplot(3,1,2)
plot(num_data,EPS(:,2))
xlabel('Numero de trame')
ylabel('Icharge (A)')
grid on
subplot(3,1,3)
plot(num_data,EPS(:,3))
xlabel('Numero de trame')
ylabel('Temperature batteries (°C)')
grid on

figure
plot(num_data,Temp(:,1))
title('Temperature')
xlabel('Numero de trame')
ylabel('Temperature (°C)')
grid on

figure
plot(num_data,Euler(:,1),'r',num_data,Euler(:,2),'b',num_data,Euler(:,3),'g')
title('Euler')
xlabel('Numero de trame')
ylabel('angle en °')
legend('roulis', 'tangage', 'lacet')
grid on

##figure
##plot(num_data,Quaternion(:,1),'r',num_data,Quaternion(:,2),'b',num_data,Quaternion(:,3),'g',Quaternion(:,4),'p')
##title('Quaternion')
##xlabel('Numero de trame')
##ylabel('valeur quaternion')


figure
plot(num_data,v_angulaire(:,1),'r',num_data,v_angulaire(:,2),'b',num_data,v_angulaire(:,3),'g')
title('V angulaire')
xlabel('Numero de trame')
ylabel('vitesse angulaire (°/s)')
legend('Rx', 'Ry', 'Rz')
grid on

figure
plot(num_data,accel(:,1),'r',num_data,accel(:,2),'b',num_data,accel(:,3),'g')
title('acceleration')
xlabel('Numero de trame')
ylabel('acceleration (m/s^2)')
legend('ax','ay','az')
grid on

figure
plot(num_data,chp_magn(:,1),'r',num_data,chp_magn(:,2),'b',num_data,chp_magn(:,3),'g')
title('Champ magnetique')
xlabel('Numero de trame')
ylabel('champ magnetique (T)')
legend('Bx','By','Bz')
grid on

figure
plot(num_data,gravite(:,1),'r',num_data,gravite(:,2),'b',num_data,gravite(:,3),'g')
title('gravite')
xlabel('Numero de trame')
ylabel('gravite (m/s^2)')
legend('Gx','Gy','Gz')
grid on

figure
plot(num_data,luminance(:,1),'r',num_data,luminance(:,2),'b',num_data,luminance(:,3),'g',num_data,luminance(:,4),'p',num_data,luminance(:,5),'y')
title('luminance')
xlabel('Numero de trame')
ylabel('luminance')
legend('-X','+X','-Y','+Y','+Z')
grid on

figure
plot(GNSS(:,2),GNSS(:,1))
title('GNSS')
xlabel('Longitude')
ylabel('Latitude')
grid on

if columns > 35
figure
plot(num_data,Rssi(:,1));
title('Rssi')
xlabel('Numero de trame')
ylabel('Rssi en dBm')
grid on
end