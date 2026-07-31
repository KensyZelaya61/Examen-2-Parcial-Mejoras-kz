#include <iostream>      
#include <string>       
#include <cmath>        
#include <fstream>    
#include <iomanip>      

using namespace std;

// Funcion auxiliar para leer respuestas de tipo Si/No de forma validada.
char leerRespuestaSN(const string &mensaje) {
	char resp;
	do {
		cout << mensaje;
		cin >> resp;
		resp = toupper(resp);
		if (resp != 'S' && resp != 'N') {
			cout << "Opcion invalida. Por favor ingrese S (Si) o N (No).\n";
		}
	} while (resp != 'S' && resp != 'N');
	return resp;
}

int main() {
	// Datos del inventario con vectores
	const int NUM_PRODUCTOS = 12;
	int codigos[NUM_PRODUCTOS] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
	
	string nombres[NUM_PRODUCTOS] = {
		"Filtro de aceite", "Bujia (unidad)", "Aceite 10W-30 (litro)",
			"Pastillas de freno (juego)", "Disco de freno (unidad)", "Amortiguador (unidad)",
			"Bateria 12V", "Alternador", "Bombillo LED (unidad)",
			"Limpiaparabrisas (par)", "Aromatizante", "Cera para auto"
	};
	
	string categorias[NUM_PRODUCTOS] = {
		"Motor y Afinacion", "Motor y Afinacion", "Motor y Afinacion",
			"Frenos y Suspension", "Frenos y Suspension", "Frenos y Suspension",
			"Electrico", "Electrico", "Electrico",
			"Accesorios", "Accesorios", "Accesorios"
	};
	
	double precios[NUM_PRODUCTOS] = {
		120.00, 90.00, 180.00, 450.00, 900.00, 1200.00, 
			2500.00, 3800.00, 80.00, 250.00, 60.00, 150.00
	};
	
	int inventario[NUM_PRODUCTOS] = {30, 80, 40, 25, 15, 12, 10, 6, 60, 20, 50, 25};
	int puntosProducto[NUM_PRODUCTOS] = {10, 8, 12, 25, 40, 50, 80, 100, 5, 15, 3, 8};
	
	// Arreglo de los 5 bancos disponibles
	string bancosDisponibles[5] = {
		"Banco Ficohsa", 
			"BAC Credomatic", 
			"Banpais", 
			"Banco Atlantida", 
			"Banco Promerica"
	};
	
	// Variables para guardar lo vendido al final del dia
	int unidadesVendidas[NUM_PRODUCTOS] = {0};
	double ingresosPorProducto[NUM_PRODUCTOS] = {0.0};
	double descuentoPorProducto[NUM_PRODUCTOS] = {0.0};
	
	// Categorias: 0=Motor, 1=Frenos, 2=Electrico, 3=Accesorios
	double catIngresoBruto[4] = {0.0, 0.0, 0.0, 0.0};
	double catDescuentos[4] = {0.0, 0.0, 0.0, 0.0};
	double catIngresoNeto[4] = {0.0, 0.0, 0.0, 0.0};
	
	// Totales acumulados generales
	double totalDescuentosDia = 0;
	double totalISVDia = 0;
	double totalGeneralLpsDia = 0;
	int totalPuntosGeneradosDia = 0;
	int totalPuntosCanjeadosDia = 0;
	double totalRecargosTarjetaDia = 0;
	double totalDescuentosFinancierosDia = 0;
	
	int conteoEfectivo = 0;
	int conteoTransferencia = 0;
	int conteoTarjeta = 0;
	
	char continuarCliente;
	
	// Ciclo principal de atencion a clientes
	do {
		string nombreCliente;
		int tipoCliente;
		char genero;
		int edad;
		
		cout << "\n========================================================================\n";
		cout << "                   AUTO REPUESTOS \"PUMA MOTOR PARTS\"                    \n";
		cout << "========================================================================\n";
		
		cout << "Ingrese el nombre del cliente: ";
		getline(cin >> ws, nombreCliente); 
		
		do {
			cout << "Tipo de cliente (1: Particular [5% Accesorios], 2: Taller [10% Motor], 3: Flotilla [12% Frenos]): ";
			cin >> tipoCliente;
			if (tipoCliente < 1 || tipoCliente > 3) cout << "Opcion invalida.Debe ser entre 1 y 3\n";
		} while (tipoCliente < 1 || tipoCliente > 3);
		
		do {
			cout << "Ingrese el genero del cliente (M: [5% Frenos] / F: [10% Accesorios]): ";
			cin >> genero;
			genero = toupper(genero);
			if (genero != 'M' && genero != 'F') cout << "Genero invalido.Deber ser M masculino o F femenino\n";
		} while (genero != 'M' && genero != 'F');
		
		do {
			cout << "Ingrese la edad del cliente: ";
			cin >> edad;
			if (edad < 18 || edad > 120) cout << "Edad invalida.Debe estar entre 18 y 120\n";
		} while (edad < 18 || edad > 120);
		
		// Vectores temporales por cliente
		int productosCompradosIdx[NUM_PRODUCTOS] = {0};
		int productosCompradosCant[NUM_PRODUCTOS] = {0};
		double productosCompradosSub[NUM_PRODUCTOS] = {0.0};
		
		double dctoTipoClienteItem[NUM_PRODUCTOS] = {0.0};
		double dctoGeneroItem[NUM_PRODUCTOS] = {0.0};
		double dctoVolumenItem[NUM_PRODUCTOS] = {0.0};
		
		int totalItemsDistintos = 0;
		
		char agregarMasProductos;
		double subTotalCliente = 0;
		double totalDescuentosCliente = 0;
		int puntosClienteFactura = 0;
		
		int unidadesFiltroCliente = 0;
		int unidadesAceiteCliente = 0;
		int unidadesPastillasCliente = 0;
		int unidadesDiscoCliente = 0;
		bool categoriesCompradas[4] = {false, false, false, false};
		
		// Carrito de compras
		do {
			int codigoProducto;
			int cantidad;
			
			cout << "\n--- PRODUCTOS EN TIENDA ---\n";
			for (int i = 0; i < NUM_PRODUCTOS; i++) {
				cout << codigos[i] << ". " << left << setw(28) << nombres[i] 
					<< " [" << categorias[i] << "] - Lps. " << precios[i] << " [Stock: " << inventario[i] << "]\n";
			}
			
			do {
				cout << "\nIngrese el codigo del producto (1-12): ";
				cin >> codigoProducto;
				if (codigoProducto < 1 || codigoProducto > 12) {
					cout << "Codigo invalido. Debe estar entre 1 y 12.\n";
				}
			} while (codigoProducto < 1 || codigoProducto > 12);
			
			do {
				cout << "Ingrese la cantidad a comprar: ";
				cin >> cantidad;
				if (cantidad <= 0) {
					cout << "Cantidad invalida. Debe ser mayor a 0.\n";
				}
			} while (cantidad <= 0);
			
			int idx = codigoProducto - 1; 
			
			if (inventario[idx] == 0) {
				cout << "-> Producto agotado.\n";
			} else if (cantidad > inventario[idx]) {
				cout << "-> Inventario insuficiente. Solo quedan " << inventario[idx] << " piezas.\n";
			} else {
				inventario[idx] -= cantidad;
				
				if (codigoProducto == 1) unidadesFiltroCliente += cantidad;
				if (codigoProducto == 3) unidadesAceiteCliente += cantidad;
				if (codigoProducto == 4) unidadesPastillasCliente += cantidad;
				if (codigoProducto == 5) unidadesDiscoCliente += cantidad;
				
				int catIdx = -1;
				if (categorias[idx] == "Motor y Afinacion") catIdx = 0;
				else if (categorias[idx] == "Frenos y Suspension") catIdx = 1;
				else if (categorias[idx] == "Electrico") catIdx = 2;
				else if (categorias[idx] == "Accesorios") catIdx = 3;
				
				if (catIdx != -1) categoriesCompradas[catIdx] = true;
				
				double precioBaseItem = precios[idx] * cantidad;
				puntosClienteFactura += (puntosProducto[idx] * cantidad);
				
				double descCliente = 0, descGenero = 0, descVolumen = 0;
				
				if (tipoCliente == 1 && categorias[idx] == "Accesorios") descCliente = precioBaseItem * 0.05;
				if (tipoCliente == 2 && categorias[idx] == "Motor y Afinacion") descCliente = precioBaseItem * 0.10;
				if (tipoCliente == 3 && categorias[idx] == "Frenos y Suspension") descCliente = precioBaseItem * 0.12;
				
				if (genero == 'F' && categorias[idx] == "Accesorios") descGenero = precioBaseItem * 0.10;
				if (genero == 'M' && categorias[idx] == "Frenos y Suspension") descGenero = precioBaseItem * 0.05;
				
				if (cantidad > 5) descVolumen = precioBaseItem * 0.03;
				
				double totalDescuentoItem = descCliente + descGenero + descVolumen;
				
				productosCompradosIdx[totalItemsDistintos] = idx;
				productosCompradosCant[totalItemsDistintos] = cantidad;
				productosCompradosSub[totalItemsDistintos] = precioBaseItem;
				
				dctoTipoClienteItem[totalItemsDistintos] = descCliente;
				dctoGeneroItem[totalItemsDistintos] = descGenero;
				dctoVolumenItem[totalItemsDistintos] = descVolumen;
				
				totalItemsDistintos++;
				
				subTotalCliente += precioBaseItem;
				totalDescuentosCliente += totalDescuentoItem;
				unidadesVendidas[idx] += cantidad;
				descuentoPorProducto[idx] += totalDescuentoItem;
				ingresosPorProducto[idx] += (precioBaseItem - totalDescuentoItem);
				
				if (catIdx != -1) {
					catIngresoBruto[catIdx] += precioBaseItem;
					catDescuentos[catIdx] += totalDescuentoItem;
				}
				cout << "-> Agregado al carrito correctamente.\n";
			}
			
			agregarMasProductos = leerRespuestaSN("Desea comprar otro producto para este cliente? (S/N): ");
			
		} while (agregarMasProductos == 'S');
		
		if (subTotalCliente <= 0) {
			cout << "\nCarrito vacio. Pasando al siguiente.\n";
			continuarCliente = leerRespuestaSN("Desea ingresar otro cliente? (S/N): ");
			continue; 
		}
		
		// Calculo de Descuentos Especiales y Combos
		double descuentoCombo = 0;
		int combosAceite = (unidadesFiltroCliente < unidadesAceiteCliente) ? unidadesFiltroCliente : unidadesAceiteCliente;
		if (combosAceite > 0) {
			descuentoCombo += combosAceite * (precios[0] + precios[2]) * 0.08;
			catDescuentos[0] += combosAceite * (precios[0] + precios[2]) * 0.08;
		}
		
		int combosFrenos = (unidadesPastillasCliente < unidadesDiscoCliente) ? unidadesPastillasCliente : unidadesDiscoCliente;
		if (combosFrenos > 0) {
			descuentoCombo += combosFrenos * (precios[3] + precios[4]) * 0.06;
			catDescuentos[1] += combosFrenos * (precios[3] + precios[4]) * 0.06;
		}
		
		double descuentoCompraMayor = 0;
		double pctCompraMayor = 0;
		if (subTotalCliente > 10000.0) { descuentoCompraMayor = subTotalCliente * 0.04; pctCompraMayor = 4.0; }
		else if (subTotalCliente > 5000.0) { descuentoCompraMayor = subTotalCliente * 0.02; pctCompraMayor = 2.0; }
		
		double descuentoMultiCategoria = 0;
		int totalCategoriasTocadas = 0;
		for (int c = 0; c < 4; c++) {
			if (categoriesCompradas[c]) totalCategoriasTocadas++;
		}
		if (totalCategoriasTocadas >= 3) {
			descuentoMultiCategoria = subTotalCliente * 0.03;
		}
		
		double descuentoTercera = 0;
		if (edad >= 60) {
			descuentoTercera = subTotalCliente * 0.10;
		}
		
		double totalDescuentosDeducciones = descuentoCombo + descuentoCompraMayor + descuentoMultiCategoria + descuentoTercera;
		totalDescuentosCliente += totalDescuentosDeducciones;
		
		for (int c = 0; c < 4; c++) {
			if (categoriesCompradas[c] && subTotalCliente > 0) {
				double prorrateo = catIngresoBruto[c] / subTotalCliente;
				catDescuentos[c] += totalDescuentosDeducciones * prorrateo;
			}
		}
		
		double totalConDescuento = subTotalCliente - totalDescuentosCliente;
		if (totalConDescuento < 0) totalConDescuento = 0;
		
		// Canje de Puntos
		double rebajaPuntos = 0;
		if (puntosClienteFactura >= 100) {
			cout << "\n>>> El cliente tiene " << puntosClienteFactura << " puntos acumulables en esta venta.\n";
			char opcionCanje = leerRespuestaSN("Desea aplicar canje de puntos por dinero? (S/N): ");
			if (opcionCanje == 'S') {
				int puntosGastados = 0;
				if (puntosClienteFactura >= 300) { rebajaPuntos = 200.0; puntosGastados = 300; }
				else if (puntosClienteFactura >= 200) { rebajaPuntos = 120.0; puntosGastados = 200; }
				else if (puntosClienteFactura >= 100) { rebajaPuntos = 50.0; puntosGastados = 100; }
				
				totalConDescuento -= rebajaPuntos;
				if (totalConDescuento < 0) totalConDescuento = 0;
				
				totalPuntosCanjeadosDia += puntosGastados;
				totalDescuentosFinancierosDia += rebajaPuntos;
			}
		}
		totalPuntosGeneradosDia += puntosClienteFactura;
		
		// Impuestos y Totales
		double isv = totalConDescuento * 0.15;
		double totalNetoLps = totalConDescuento + isv;
		double totalNetoUSD = totalNetoLps / 26.42;
		
		// --- NOTIFICACION DE BENEFICIOS Y DESCUENTOS ---
		cout << "\n======================================================================================\n";
		cout << "                 RESUMEN DE BENEFICIOS Y DESCUENTOS APLICADOS                         \n";
		cout << "======================================================================================\n";
		cout << "Cliente: " << nombreCliente << " | Perfil: " 
			<< (tipoCliente == 1 ? "Particular" : (tipoCliente == 2 ? "Taller" : "Flotilla")) 
			<< " | Genero: " << genero << " | Edad: " << edad << "\n";
		cout << "--------------------------------------------------------------------------------------\n";
		
		if (tipoCliente == 1) cout << " -> [Perfil] Descuento Particular: 5% en la categoria Accesorios.\n";
		if (tipoCliente == 2) cout << " -> [Perfil] Descuento Taller: 10% en la categoria Motor y Afinacion.\n";
		if (tipoCliente == 3) cout << " -> [Perfil] Descuento Flotilla: 12% en la categoria Frenos y Suspension.\n";
		
		if (genero == 'F') cout << " -> [Genero] Beneficio Femenino: 10% extra en Accesorios.\n";
		if (genero == 'M') cout << " -> [Genero] Beneficio Masculino: 5% extra en Frenos y Suspension.\n";
		
		if (descuentoCombo > 0) cout << " -> [Combo Especial] Ahorro de Lps. " << descuentoCombo << " aplicados en combos de repuestos.\n";
		if (descuentoCompraMayor > 0) cout << " -> [Volumen Compra] Descuento de " << pctCompraMayor << "% aplicado por compra superior. (Lps. " << descuentoCompraMayor << ")\n";
		if (descuentoMultiCategoria > 0) cout << " -> [Multi-Categoria] Descuento del 3% extra por llevar 3 o mas categorias diferentes. (Lps. " << descuentoMultiCategoria << ")\n";
		if (descuentoTercera > 0) cout << " -> [Tercera Edad] Descuento preferencial del 10% activado. (Lps. " << descuentoTercera << ")\n";
		if (rebajaPuntos > 0) cout << " -> [Canje Puntos] Reduccion directa de Lps. " << rebajaPuntos << " canjeando puntos.\n";
		
		cout << " -> [Fidelidad] Se acumularon " << puntosClienteFactura << " puntos en esta operacion.\n";
		cout << "======================================================================================\n";
		
		// --- IMPRESION DE FACTURA PREVIA AL PAGO ---
		cout << "\n======================================================================================\n";
		cout << "                                FACTURA FORMAL DE VENTA                               \n";
		cout << "======================================================================================\n";
		cout << "CLIENTE      : " << nombreCliente << "\t\tEDAD: " << edad << "  \tSEXO: " << genero << "\n";
		cout << "TIPO CLIENTE : " << (tipoCliente == 1 ? "Particular" : (tipoCliente == 2 ? "Taller" : "Flotilla")) << "\n";
		cout << "--------------------------------------------------------------------------------------\n";
		cout << left << setw(23) << "Producto" << setw(6) << "Cant" << setw(11) << "P.Unit" << setw(11) << "D.Cliente" << setw(11) << "D.Genero" << setw(11) << "D.Vol(3%)" << setw(12) << "Subtotal" << "\n";
		cout << "--------------------------------------------------------------------------------------\n";
		
		for (int i = 0; i < totalItemsDistintos; i++) {
			int pIdx = productosCompradosIdx[i];
			cout << left << setw(23) << nombres[pIdx].substr(0, 22)
				<< setw(6)  << productosCompradosCant[i]
				<< setw(11) << precios[pIdx]
				<< setw(11) << dctoTipoClienteItem[i]
				<< setw(11) << dctoGeneroItem[i]
				<< setw(11) << dctoVolumenItem[i]
				<< setw(12) << productosCompradosSub[i] << "\n";
		}
		cout << "--------------------------------------------------------------------------------------\n";
		cout << "RESUMEN MONETARIO:\n";
		cout << "    (+) Subtotal Bruto                     : Lps. " << subTotalCliente << "\n";
		cout << "    (-) Descuentos Fijos Totales           : Lps. " << (totalDescuentosCliente - totalDescuentosDeducciones) << "\n";
		if (descuentoCombo > 0)         cout << "    (-) Descuento por Combos               : Lps. " << descuentoCombo << "\n";
		if (descuentoCompraMayor > 0)   cout << "    (-) Descuento por Volumen (" << pctCompraMayor << "%)        : Lps. " << descuentoCompraMayor << "\n";
		if (descuentoMultiCategoria > 0) cout << "    (-) Descuento Multi-Categoria (3%)     : Lps. " << descuentoMultiCategoria << "\n";
		if (descuentoTercera > 0)       cout << "    (-) Descuento Tercera Edad (10%)       : Lps. " << descuentoTercera << "\n";
		if (rebajaPuntos > 0)          cout << "    (-) Rebaja por Canje de Puntos         : Lps. " << rebajaPuntos << "\n";
		
		cout << "    (=) Importe Gravable Neto              : Lps. " << totalConDescuento << "\n";
		cout << "    (+) ISV / Impuesto de Ventas (15%)    : Lps. " << isv << "\n";
		cout << "--------------------------------------------------------------------------------------\n";
		cout << "    TOTAL A PAGAR (LEMPIRAS)               : Lps. " << totalNetoLps << "\n";
		cout << "    TOTAL EN DOLARES (USD)                 : USD$ " << totalNetoUSD << "\n";
		cout << "======================================================================================\n";
		
		// --- PROCESO DE PAGO Y SELECCION DE BANCOS ---
		int metodoPago;
		double recargoTarjeta = 0;
		string refBancaria = "N/A", bcoOrigen = "N/A", bcoDestino = "N/A", ctaReceptora = "N/A";
		string tarjetaNum = "N/A", tarjetaBanco = "N/A";
		int tarjetaCuotas = 0;
		
		do {
			cout << "\n--- SELECCIONE LA FORMA DE PAGO ---\n";
			cout << "1. Efectivo\n2. Transferencia Bancaria\n3. Tarjeta de Credito/Debito (5% Recargo)\n";
			cout << "Opcion: "; cin >> metodoPago;
			if (metodoPago < 1 || metodoPago > 3) {
				cout << "Opcion invalida. Debe elegir 1, 2 o 3.\n";
			}
		} while (metodoPago < 1 || metodoPago > 3);
		
		if (metodoPago == 1) { 
			double montoRecibido;
			do {
				cout << "\n[EFECTIVO] Total: Lps. " << totalNetoLps << " | Ingrese efectivo recibido: Lps. ";
				cin >> montoRecibido;
				if (montoRecibido < totalNetoLps) cout << "Dinero insuficiente. Intente de nuevo.\n";
			} while (montoRecibido < totalNetoLps);
			cout << "-> Cambio a entregar: Lps. " << (montoRecibido - totalNetoLps) << "\n";
			conteoEfectivo++;
		} 
		else if (metodoPago == 2) { 
			double montoTransferido;
			int opcBcoOrigen = 0, opcBcoDestino = 0;
			
			cout << "\n--- PROCESANDO TRANSFERENCIA BANCARIA ---\n";
			
			// SELECCION DE BANCO EMISOR DEL CLIENTE
			do {
				cout << "\nSeleccione el Banco Emisor del cliente:\n";
				for (int b = 0; b < 5; b++) {
					cout << b + 1 << ". " << bancosDisponibles[b] << "\n";
				}
				cout << "Opcion (1-5): ";
				cin >> opcBcoOrigen;
				if (opcBcoOrigen < 1 || opcBcoOrigen > 5) {
					cout << "Opcion invalida. Debe elegir un banco entre 1 y 5.\n";
				}
			} while (opcBcoOrigen < 1 || opcBcoOrigen > 5);
			bcoOrigen = bancosDisponibles[opcBcoOrigen - 1];
			
			// SELECCION DE BANCO DESTINO DE LA EMPRESA
			do {
				cout << "\nSeleccione el Banco Destino (Cuenta Puma Motors):\n";
				for (int b = 0; b < 5; b++) {
					cout << b + 1 << ". " << bancosDisponibles[b] << "\n";
				}
				cout << "Opcion (1-5): ";
				cin >> opcBcoDestino;
				if (opcBcoDestino < 1 || opcBcoDestino > 5) {
					cout << "Opcion invalida. Debe elegir un banco entre 1 y 5.\n";
				}
			} while (opcBcoDestino < 1 || opcBcoDestino > 5);
			bcoDestino = bancosDisponibles[opcBcoDestino - 1];
			
			cout << "Ingrese No. de Cuenta/Celular Receptor : "; cin >> ctaReceptora;
			cout << "Ingrese No. de Referencia/Transaccion  : "; cin >> refBancaria;
			
			cout << "\n--> VALOR EXACTO A TRANSFERIR: Lps. " << totalNetoLps << "\n";
			
			do {
				cout << "Confirme el monto de la transferencia realizada: Lps. ";
				cin >> montoTransferido;
				
				if (abs(montoTransferido - totalNetoLps) > 0.01) {
					cout << "[ERROR] El valor ingresado no coincide exactamente con el total a pagar. Reintente.\n";
				}
			} while (abs(montoTransferido - totalNetoLps) > 0.01);
			
			cout << "-> Transferencia desde " << bcoOrigen << " hacia " << bcoDestino 
				<< " verificada exitosamente. Ref: " << refBancaria << "\n";
			conteoTransferencia++;
		}
		else if (metodoPago == 3) { 
			cout << "\n--- PROCESANDO PAGO CON TARJETA ---\n";
			cout << "Numero de tarjeta (ultimos 4 digitos): "; cin >> tarjetaNum;
			
			int opcBcoTarjeta = 0;
			do {
				cout << "\nSeleccione el Banco emisor de la tarjeta:\n";
				for (int b = 0; b < 5; b++) {
					cout << b + 1 << ". " << bancosDisponibles[b] << "\n";
				}
				cout << "Opcion (1-5): ";
				cin >> opcBcoTarjeta;
				if (opcBcoTarjeta < 1 || opcBcoTarjeta > 5) {
					cout << "Opcion invalida. Debe elegir un banco entre 1 y 5.\n";
				}
			} while (opcBcoTarjeta < 1 || opcBcoTarjeta > 5);
			tarjetaBanco = bancosDisponibles[opcBcoTarjeta - 1];
			
			// --- VALIDACION AGREGADA: cuotas debe estar entre 1 y 12 ---
			do {
				cout << "Cantidad de cuotas elegidas (1-12)   : "; 
				cin >> tarjetaCuotas;
				if (tarjetaCuotas < 1 || tarjetaCuotas > 12) {
					cout << "Cantidad de cuotas invalida. Debe estar entre 1 y 12.\n";
				}
			} while (tarjetaCuotas < 1 || tarjetaCuotas > 12);
			
			recargoTarjeta = totalNetoLps * 0.05;
			totalNetoLps += recargoTarjeta;
			totalNetoUSD = totalNetoLps / 26.42;
			totalRecargosTarjetaDia += recargoTarjeta;
			
			cout << "-> Recargo por uso de tarjeta (5%): Lps. " << recargoTarjeta << "\n";
			cout << "-> NUEVO TOTAL FINAL COBRADO    : Lps. " << totalNetoLps << "\n";
			conteoTarjeta++;
		}
		
		totalDescuentosDia += totalDescuentosCliente;
		totalISVDia += isv;
		totalGeneralLpsDia += totalNetoLps;
		
		continuarCliente = leerRespuestaSN("\nDesea ingresar otro cliente al sistema? (S/N): ");
		
	} while (continuarCliente == 'S');
	
	// Estadisticas finales
	int idxMasVendido = 0, idxMenosVendido = -1;
	int idxMayorIngreso = 0, idxMayorDescuento = 0, idxMenorInventario = 0;
	
	for (int i = 0; i < NUM_PRODUCTOS; i++) {
		if (unidadesVendidas[i] > unidadesVendidas[idxMasVendido]) idxMasVendido = i;
		
		// Buscar el producto menos vendido entre los que SÍ tuvieron ventas (> 0)
		if (unidadesVendidas[i] > 0) {
			if (idxMenosVendido == -1 || unidadesVendidas[i] < unidadesVendidas[idxMenosVendido]) {
				idxMenosVendido = i;
			}
		}
		
		if (ingresosPorProducto[i] > ingresosPorProducto[idxMayorIngreso]) idxMayorIngreso = i;
		if (descuentoPorProducto[i] > descuentoPorProducto[idxMayorDescuento]) idxMayorDescuento = i;
		if (inventario[i] < inventario[idxMenorInventario]) idxMenorInventario = i;
	}
	
	string metodoMasUtilizado = "Ninguno / Empate";
	if (conteoEfectivo > conteoTransferencia && conteoEfectivo > conteoTarjeta) metodoMasUtilizado = "Efectivo";
	else if (conteoTransferencia > conteoEfectivo && conteoTransferencia > conteoTarjeta) metodoMasUtilizado = "Transferencia Bancaria";
	else if (conteoTarjeta > conteoEfectivo && conteoTarjeta > conteoTransferencia) metodoMasUtilizado = "Tarjeta de Credito/Debito";
	
	for (int c = 0; c < 4; c++) {
		catIngresoNeto[c] = catIngresoBruto[c] - catDescuentos[c];
		if (catIngresoNeto[c] < 0) catIngresoNeto[c] = 0;
	}
	
	// Reporte en pantalla con formato de 2 decimales
	cout << fixed << setprecision(2);
	
	cout << "\n========================================================================================\n";
	cout << "                            CIERRE DE CAJA Y ESTADISTICAS                               \n";
	cout << "========================================================================================\n";
	cout << left << setw(25) << "Categoria" << setw(20) << "Ingreso Bruto" << setw(20) << "Descuentos" << setw(20) << "Ingreso Neto" << "\n";
	cout << "----------------------------------------------------------------------------------------\n";
	cout << left << setw(25) << "0. Motor y Afinacion" << "Lps. " << left << setw(15) << catIngresoBruto[0] << "Lps. " << left << setw(15) << catDescuentos[0] << "Lps. " << left << setw(15) << catIngresoNeto[0] << "\n";
	cout << left << setw(25) << "1. Frenos y Suspension" << "Lps. " << left << setw(15) << catIngresoBruto[1] << "Lps. " << left << setw(15) << catDescuentos[1] << "Lps. " << left << setw(15) << catIngresoNeto[1] << "\n";
	cout << left << setw(25) << "2. Electrico" << "Lps. " << left << setw(15) << catIngresoBruto[2] << "Lps. " << left << setw(15) << catDescuentos[2] << "Lps. " << left << setw(15) << catIngresoNeto[2] << "\n";
	cout << left << setw(25) << "3. Accesorios" << "Lps. " << left << setw(15) << catIngresoBruto[3] << "Lps. " << left << setw(15) << catDescuentos[3] << "Lps. " << left << setw(15) << catIngresoNeto[3] << "\n";
	cout << "----------------------------------------------------------------------------------------\n\n";
	
	cout << "1. Producto mas vendido            : " << nombres[idxMasVendido] << " (" << unidadesVendidas[idxMasVendido] << " unidades)\n";
	if (idxMenosVendido != -1) {
		cout << "2. Producto menos vendido          : " << nombres[idxMenosVendido] << " (" << unidadesVendidas[idxMenosVendido] << " unidades)\n";
	} else {
		cout << "2. Producto menos vendido          : Ninguna venta registrada\n";
	}
	cout << "3. Ganancia neta por categoria     : (Ver tabla superior)\n";
	cout << "4. Total descuentos aplicados      : Lps. " << totalDescuentosDia << "\n";
	cout << "5. Total ISV recaudado (15%)       : Lps. " << totalISVDia << "\n";
	cout << "6. Total vendido en Lempiras       : Lps. " << totalGeneralLpsDia << "\n";
	cout << "7. Total vendido en Dolares        : USD$ " << (totalGeneralLpsDia / 26.42) << "\n";
	cout << "8. Metodo de pago mas comun        : " << metodoMasUtilizado << "\n";
	cout << "9. Producto con mayor ingreso      : " << nombres[idxMayorIngreso] << " (Lps. " << ingresosPorProducto[idxMayorIngreso] << ")\n";
	cout << "10. Producto con mayor descuento   : " << nombres[idxMayorDescuento] << " (Lps. " << descuentoPorProducto[idxMayorDescuento] << ")\n";
	cout << "11. Producto con stock mas bajo    : " << nombres[idxMenorInventario] << " (" << inventario[idxMenorInventario] << " en tienda)\n";
	cout << "12. Total de puntos acumulados     : " << totalPuntosGeneradosDia << " pts\n";
	cout << "13. Total de puntos rebajados      : " << totalPuntosCanjeadosDia << " pts\n";
	cout << "14. Total cobrado por recargo (5%) : Lps. " << totalRecargosTarjetaDia << "\n";
	cout << "15. Total descontado por puntos    : Lps. " << totalDescuentosFinancierosDia << "\n";
	cout << "========================================================================================\n";
	
	// Exportacion
	char respuestaDescarga = leerRespuestaSN("\nDesea guardar el reporte del dia en un archivo de texto? (S/N): ");
	
	if (respuestaDescarga == 'S') {
		ofstream archivoTxt("reporte_dia_puma_motor_parts.txt");
		if (archivoTxt.is_open()) {
			archivoTxt << fixed << setprecision(2);
			archivoTxt << "========================================================================================\n";
			archivoTxt << "                         REPORTE DE ESTADISTICAS DEL DIA                                \n";
			archivoTxt << "                         AUTO REPUESTOS PUMA MOTOR PARTS                                \n";
			archivoTxt << "========================================================================================\n\n";
			
			archivoTxt << "----------------------------------------------------------------------------------------\n";
			archivoTxt << " 3. REPORTE FINANCIERO POR CATEGORIA\n";
			archivoTxt << "----------------------------------------------------------------------------------------\n";
			archivoTxt << "    Categoria\t\t\tIngreso Bruto\t\tDescuentos\t\tIngreso Neto\n";
			archivoTxt << "----------------------------------------------------------------------------------------\n";
			archivoTxt << "    - Motor y Afinacion\t\tLps. " << catIngresoBruto[0] << "\t\tLps. " << catDescuentos[0] << "\t\tLps. " << catIngresoNeto[0] << "\n";
			archivoTxt << "    - Frenos y Suspension\tLps. " << catIngresoBruto[1] << "\t\tLps. " << catDescuentos[1] << "\t\tLps. " << catIngresoNeto[1] << "\n";
			archivoTxt << "    - Electrico\t\t\tLps. " << catIngresoBruto[2] << "\t\tLps. " << catDescuentos[2] << "\t\tLps. " << catIngresoNeto[2] << "\n";
			archivoTxt << "    - Accesorios\t\t\tLps. " << catIngresoBruto[3] << "\t\tLps. " << catDescuentos[3] << "\t\tLps. " << catIngresoNeto[3] << "\n";
			archivoTxt << "----------------------------------------------------------------------------------------\n\n";
			
			archivoTxt << "----------------------------------------------------------------------------------------\n";
			archivoTxt << " METRICAS COMERCIALES Y PRODUCTIVIDAD\n";
			archivoTxt << "----------------------------------------------------------------------------------------\n";
			archivoTxt << "  1. Producto mas vendido            : " << nombres[idxMasVendido] << " (" << unidadesVendidas[idxMasVendido] << " unds)\n";
			if (idxMenosVendido != -1) {
				archivoTxt << "  2. Producto menos vendido          : " << nombres[idxMenosVendido] << " (" << unidadesVendidas[idxMenosVendido] << " unds)\n";
			} else {
				archivoTxt << "  2. Producto menos vendido          : Ninguna venta registrada\n";
			}
			archivoTxt << "  8. Producto con mayor ingreso      : " << nombres[idxMayorIngreso] << " (Lps. " << ingresosPorProducto[idxMayorIngreso] << ")\n";
			archivoTxt << "  9. Producto con mayor descuento    : " << nombres[idxMayorDescuento] << " (Lps. " << descuentoPorProducto[idxMayorDescuento] << ")\n";
			archivoTxt << " 10. Producto con menor inventario   : " << nombres[idxMenorInventario] << " (" << inventario[idxMenorInventario] << " unds restantes)\n";
			archivoTxt << "----------------------------------------------------------------------------------------\n\n";
			
			archivoTxt << "----------------------------------------------------------------------------------------\n";
			archivoTxt << " BALANCE COBROS, IMPUESTOS Y FIDELIZACION\n";
			archivoTxt << "----------------------------------------------------------------------------------------\n";
			archivoTxt << "  4. Total descuentos aplicados      : Lps. " << totalDescuentosDia << "\n";
			archivoTxt << "  5. Total ISV recaudado (15%)       : Lps. " << totalISVDia << "\n";
			archivoTxt << "  6. Total vendido en Lempiras       : Lps. " << totalGeneralLpsDia << "\n";
			archivoTxt << "     Total vendido en Dolares        : USD$ " << (totalGeneralLpsDia / 26.42) << "\n";
			archivoTxt << "  7. Metodo de pago mas utilizado    : " << metodoMasUtilizado 
				<< " (Efectivo: " << conteoEfectivo << ", Transferencia: " << conteoTransferencia << ", Tarjeta: " << conteoTarjeta << ")\n";
			archivoTxt << " 11. Total de puntos generados       : " << totalPuntosGeneradosDia << " pts\n";
			archivoTxt << " 12. Total de puntos canjeados       : " << totalPuntosCanjeadosDia << " pts\n";
			archivoTxt << " 13. Total de recargos por tarjeta   : Lps. " << totalRecargosTarjetaDia << "\n";
			archivoTxt << " 14. Total de descuentos financieros : Lps. " << totalDescuentosFinancierosDia << " (por canje)\n";
			archivoTxt << "----------------------------------------------------------------------------------------\n";
			archivoTxt << " Total de facturas emitidas en el dia: " << (conteoEfectivo + conteoTransferencia + conteoTarjeta) << "\n";
			archivoTxt << "========================================================================================\n";
			
			archivoTxt.close();
			cout << "\n[OK] El reporte se ha guardado exitosamente como 'reporte_dia_puma_motor_parts.txt'.\n";
		} else {
			cout << "\n[ERROR] No se pudo crear el archivo de texto.\n";
		}
	}
	
	cout << "\nPrograma terminado.\n";
	return 0;
}