#ifndef FORMS_MODEL_H 
#define FORMS_MODEL_H 

#include <string>
#include <vector>
#include <memory> // shared_ptr
using namespace std;

#include <QObject>
#include <QColor>
#include <QAbstractTableModel>

class FormsModel : public QAbstractTableModel
{
	public:

		class Entry {
			public:
				string date_filed;
				string accession_number;
				string filer_cik; /* note: Could be filer or issuer... */
				string filer_name;
				string filer_street_addr;
				string filer_city;
				string filer_state;
				string filer_zip;
				string sec_link;              

			ostream& print(ostream& stream) const { 
				stream << "date_filed: " << date_filed << "\n"
				 << "accession_number: " << accession_number << "\n"
				 << "filer_cik: " << filer_cik << "\n"
				 << "filer_name: " << filer_name << "\n"
				 << "filer_street_addr: " << filer_street_addr << "\n"
				 << "filer_city: " << filer_city << "\n"
				 << "filer_state: " << filer_state << "\n"
				 << "filer_zip: " << filer_zip << "\n"
				 << "sec_link: " << sec_link << endl;

				return stream;
			}/* Entry::print() */
		};/* class Entry */

	protected:

		friend class Comparator;

		class Comparator {
			public:
				FormsModel* m_parent;
				int m_sortColumn;		
				Qt::SortOrder m_sortOrder;

				Comparator(): m_parent(NULL), m_sortColumn(-1), m_sortOrder(Qt::AscendingOrder){}

			public:
				bool operator()(int i, int j);
		};/* class Comparator */

		int m_lastSortColumn;		
		Qt::SortOrder m_lastSortOrder;

		vector<shared_ptr<FormsModel::Entry>> m_entries;

		vector<string> m_columns;
		void setUpColumns();

		FormsModel::Comparator m_comparator; 

	public:
		const static int COL_DATE_FILED;
		const static int COL_ACCESSION_NUMBER;
		const static int COL_FILER_CIK; /* Note: Could be filer or issuer... */
		const static int COL_FILER_NAME;
		const static int COL_FILER_STREET_ADDR;
		const static int COL_FILER_CITY;
		const static int COL_FILER_STATE;
		const static int COL_FILER_ZIP;
		const static int COL_SEC_LINK;

		const static int COL_MAX;

		static string SortOrderToString( Qt::SortOrder sortOrder );

    	FormsModel(QObject *parent = 0);

		/**
		* @throws VF::Utils::Exception if has trouble loading sCapturesFilePath.
		*/ 
		void loadForms();
		
		// Overrides for QAbstractTableModel
    	int rowCount(const QModelIndex &parent) const;
	    int columnCount(const QModelIndex &parent) const;
		QVariant data(const QModelIndex &index, int role) const;
		QVariant headerData(int section, Qt::Orientation orientation, int role) const;

		//void sort ( int column, Qt::SortOrder order = Qt::AscendingOrder );

};/* class FormsModel : public QAbstractTableModel */

ostream& operator<<(ostream& s, const FormsModel::Entry& entry);


#endif /* #ifndef FORMS_MODEL_H  */
