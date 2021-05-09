# SistemProgramlamaProjesi
Sistem Programlama Bahar 2021 Proje Ödevi 
Grup Elemanları: 
Muhammed Melih Çelik – G181210023
Ceyda Önemli - G181210107
Hilal Yıldız - B181210015

Proje Durumu:
İlk önce libfdr kütüphanesini projemize dahil ettik. Makefile dosyasını ve ilk driver programımızı oluşturduk. Giriş metninin karakter ve kelime sınırlamaları kontrolüyle ve libfdr kütüphanesinden yararlanarak kelime kelime okunmasını sağladık. Bu kelimeleri bir dizide tuttuk, daha sonrasında .kilit dosyasındaki anahtar ve değerlerle karşılaştırarak ağaç yapımız ile verilerin şifrelenip veya çözülmesinde kullanacağız. .kilit dosyasındaki json veriyi anahtar ve değer olmak üzere parçaladık ve ayrı ayrı dizilerde sakladık. Bu sayede sonraki adımımızda okunan veriyi ağaca yerleştirmemiz ve ağaçtan okunan veriyi ilgili işleme göre şifrelememiz veya çözmemiz daha esnek ve kolay olacaktır. Projemizde verilerin ağaca eklenme ve okunan verinin işleme göre dosyaya yazılması kısmına geçeceğiz.
