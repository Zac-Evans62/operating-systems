Compilation: 
g++ producer.cpp -pthread -lrt -o producer
g++ consumer.cpp -pthread -lrt -o consumer

run:
./producer & ./consumer &
