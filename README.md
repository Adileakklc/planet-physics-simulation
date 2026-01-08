# planet-physics-simulation

Bu proje, **C programlama dili** kullanılarak geliştirilmiş konsol tabanlı bir fizik simülasyonudur. Projenin temel amacı, aynı fiziksel deneylerin Güneş Sistemi’ndeki farklı gezegenlerde, **yerçekimi ivmesine bağlı olarak** nasıl farklı sonuçlar ürettiğini gözlemleyebilmektir.

---

## Projenin Amacı

Fizikte sıkça kullanılan bazı temel deneylerin, gezegenlerin fiziksel koşullarına göre nasıl değiştiğini sayısal olarak incelemek ve bu farkları karşılaştırmalı şekilde sunmaktır. Bu sayede fiziksel kavramların yalnızca teorik değil, **algoritmik bir bakış açısıyla** da anlaşılması hedeflenmiştir.

---

## İçerilen Deneyler

Programda kullanıcı tarafından seçilebilen toplam **9 farklı fizik deneyi** bulunmaktadır:

- Serbest düşme deneyi  
- Yukarı atış deneyi  
- Ağırlık deneyi  
- Kütleçekim potansiyel enerji deneyi  
- Hidrostatik basınç deneyi  
- Arşimet kaldırma kuvveti deneyi  
- Basit sarkaç periyodu deneyi  
- Sabit ip gerilmesi deneyi  
- Asansör deneyi  

Deneyler, **Merkür, Venüs, Dünya, Mars, Jüpiter, Satürn, Uranüs ve Neptün** gezegenleri için ayrı ayrı hesaplanmaktadır.

---

##  Programın Çalışma Mantığı

Program çalıştırıldığında sırasıyla aşağıdaki adımlar izlenir:

1. Kullanıcıdan bilim insanı adı alınır.  
2. Deney menüsü ekrana basılır.  
3. Kullanıcı yapmak istediği deneyi seçer.  
4. Seçilen deneye ait gerekli metrik değerler kullanıcıdan alınır.  
5. Deney, tüm gezegenler için hesaplanır.  
6. Sonuçlar gezegen isimleriyle birlikte konsol ekranına yazdırılır.

Girilen veriler kontrol edilerek okunur. Geçersiz veya sayısal olmayan girişlerde kullanıcı uyarılır ve giriş tekrar istenir. Negatif girilen değerler ise güvenli hesaplama yapılabilmesi için mutlak değere çevrilir.

---

##  Risk Analizi

Hesaplanan deney sonuçları, gezegenler arasındaki farkların daha net görülebilmesi amacıyla **basit bir Z-score yaklaşımı** kullanılarak değerlendirilir.  
Her gezegen için elde edilen sonuçlar:

- **DÜŞÜK**
- **ORTA**
- **YÜKSEK**

risk seviyeleriyle etiketlenir. Böylece kullanıcı, yalnızca sayısal sonuçları değil, bu sonuçların gezegenler arasındaki göreli durumunu da kolayca yorumlayabilir.

