# Algoritma Dasar 
  Yang pertama bentuk dulu pakai array atau vector 3D Dimensi 1 isinya untuk randoman bentuk tetromino yang akan keluar, Dimensi 2 dan Dimensi 3 nya berguna
untuk menggambar bentuk tetromino yang akan digambar dimana yang angka 1 akan digambar sedangkan yang angka 0 tidak akan dimunculkan dalam layar. Untuk jatuhnya
ketika tetromino nya menyentuh tanah saya menyalin isi tetromino yang isinya 1 ke dalam array arena atau map namun saya salin menjadi angka 7. Alasan pakai array
peta integer karena mudah saat mengatur atau dalam pengecekan input nya karena kalau string nulis petik dua (" ") sungguh menjengkelkan oleh karena itu kita memakai 
prosedur Draw untuk menggambar game kita

# FUNCTION canMove - buat ngecek kapan si tetromino berhenti jatuh 
Cara kerja:
1. Yang pertama aku buat loop untuk mengamvil tetromino yang isinya 1 dulu lalu kita ikuti pergerakan turun nya seperti normal nya
2. Kalau salah satu bagian tetromino yang berisi angka 1 sudah menyentuh tanah sehingga kalau sudah nyentuh tanah dia bakal berhentiin
   loop yang ada di main dan main sendiri akan menggambar nya di posisi sebelum menyentuh tanah nya

# FUNCTION canTurnLeft dan canTurnRight - untuk pengecekan gerak kanan atau kiri 
Cara kerja:
1. Pertama kita loop bentuk tetromino
2. Untuk tetromino yang berisi 1 aku tambah dengan posisi Y nya dan jika di kiri atau kanan sudah ada
   balok tetromino lain maka function ini akan melarang kbhit() atau melarang player bergerak kanan atau kiri
-- Note: "Disini saya memisah function untuk pengecekan kiri atau kanan agar tidak ribet saja" --

# FUNCTION canRotate - untuk mencegah apakah tetromino dapat diputar
Cara kerja:
1. Yang pertama kita putar dulu bentuk tetromino nya  
2. Yang kedua kita mengecek area sekitar tetromino setelah diputar apakah menyentuh tanah atau border atau
   tetromino yang sudah jatuh jika dapat diputar maka fungsi akan mengembalikan nilai true yang berarti dapat diputar
4. Yang terakhir setelah mendapat validasi baru tetromino yang lama di ubah menjadi tetromino yang sudah diputar

# PROCEDURE rotateMatrix - untuk memutar bentuk tetromino
1. Yang pertama cek dulu ukuran tetromino baris x kolom
2. Selanjutnya, buat tetromino dengan ukuran kolom menjadi baris dan baris menjadi kolom (kolom x baris)
3. Lalu pindah index-index array sesuai dengan kondisi yang sudah diputar 

# PROCEDURE hardDrop - buat agar jika dipencet 's' tetromino nya langsung jatuh 
Cara kerja: 
1. Make function canMove buat ngecek pada ketinggian berapa dia bakal menyentuh tanah
2. Kalau udah ketemu digambar bayangan nya dengan loop sesuai bentuk tetromino yag jatuh
3. Cara kerja jatuhnya yang diatas selama ada bentuk nya atau angka 1 bentuknya dihilangkan atau 1 nya diganti 0 lalu ada variable
   Pass By Refference untuk ngubah posisi y tetromino yang lagi jatuh agar langsung ke tanah
