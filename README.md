# Laboratorio 2 Conway's Game of Life

Para este laboratorio de Gráficas por Computadora se puso en práctica el renderizado en tiempo real utilizando el algoritmo de Conway’s Game Of Life utilizando SDL. 
<br>
Este algoritmo se trada de un autómata celular, donde un modelo matemático simula un sistema en el que cada célula en una cuadrícula puede estar viva o muerta, y evoluciona en el tiempo siguiendo reglas simples. En este juego, las células están representadas como píxeles en una ventana gráfica. Las células pueden estar en dos estados: vivas, representadas por píxeles blancos o de colores, o muertas, representadas por píxeles negros. 
<br><br>
La evolución de las células se basa en reglas simples que siguen el siguiente patrón:<br>
1. Cualquier célula viva con menos de dos vecinos vivos muere por soledad.<br>
2. Cualquier célula viva con dos o tres vecinos vivos sigue viva para la siguiente generación.<br>
3. Cualquier célula viva con más de tres vecinos vivos muere por sobrepoblación. <br>
4. Cualquier célula muerta con exactamente tres vecinos vivos se convierte en una célula viva por reproducción <br>
<br>
Al inicio del programa, la pantalla se llena de patrones iniciales los cuales interactuarán y evolucionarán de acuerdo con las reglas del juego. Con el tiempo las células evolucionan y forman estructuras interesantes y patrones en movimiento que cambian constantemente. Cada vez que se ejecute el programa, el resultado será diferente debido a la aleatoriedad en la ubicación de los patrones iniciales.<br><br>

![Lab2](https://github.com/LINDAINES213/Lab02_Graficas/assets/77686175/153a7c8b-5d15-4be7-808a-339c4cd52051)
