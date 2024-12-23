import networkx as nx
import matplotlib.pyplot as plt

def baca_transaksi(file_path):
    transaksi = []
    with open(file_path, 'r') as file:
        for line in file:
            data = line.strip().split(',')
            transaksi.append({
                "id_transaksi": int(data[0]),
                "judul_buku": data[1],
                "metode": data[2],
            })
    return transaksi

# Membuat graf
def buat_graf(transaksi):
    G = nx.Graph()

    # Menambahkan node
    for t in transaksi:
        G.add_node(t["id_transaksi"], label=t["judul_buku"])

    # Menambahkan edge berdasarkan aturan (contoh: metode pembayaran atau judul buku yang sama)
    for i in range(len(transaksi)):
        for j in range(i + 1, len(transaksi)):
            # Hubungkan node jika metode pembayaran sama atau judul buku sama
            if (transaksi[i]["metode"] == transaksi[j]["metode"] or
                transaksi[i]["judul_buku"] == transaksi[j]["judul_buku"]):
                G.add_edge(transaksi[i]["id_transaksi"], transaksi[j]["id_transaksi"])

    return G

# Menampilkan graf
def tampilkan_graf(G):
    pos = nx.spring_layout(G)
    labels = nx.get_node_attributes(G, 'label')
    nx.draw(G, pos, with_labels=True, node_color='skyblue', node_size=1500, font_size=10, font_weight='bold')
    nx.draw_networkx_labels(G, pos, labels=labels)
    plt.title("Graf Transaksi")
    plt.show()

# Main
if __name__ == "__main__":
    file_path = "history.txt"
    transaksi = baca_transaksi(file_path)
    G = buat_graf(transaksi)
    tampilkan_graf(G)