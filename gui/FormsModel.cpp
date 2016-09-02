#include "FormsModel.h"

#include <QBrush>

#include "logger.h"

const /* static */ int FormsModel::COL_DATE_FILED = 0;
const /* static */ int FormsModel::COL_ACCESSION_NUMBER = 1;
const /* static */ int FormsModel::COL_FILER_CIK = 2; /* Note: Could be filer or issuer... */
const /* static */ int FormsModel::COL_FILER_NAME = 3;
const /* static */ int FormsModel::COL_FILER_STREET_ADDR = 4;
const /* static */ int FormsModel::COL_FILER_CITY = 5;
const /* static */ int FormsModel::COL_FILER_STATE = 6;
const /* static */ int FormsModel::COL_FILER_ZIP = 7;
const /* static */ int FormsModel::COL_SEC_LINK = 8;

const /* static */ int FormsModel::COL_MAX = FormsModel::COL_SEC_LINK;

FormsModel::FormsModel(QObject *parent) 
    : QAbstractTableModel(parent)
{
	//const char* sWho = "FormsModel::FormsModel";
	//VF::Logger::log(VF::Logger::INFO) << sWho << "(): Calling setUpColumns()...";
	m_comparator.m_parent = this;

	m_lastSortColumn = 0;
	m_lastSortOrder = Qt::AscendingOrder;

	setUpColumns();	
}/* FormsModel::FormsModel(QObject *parent) */

/* static */ string FormsModel::SortOrderToString( Qt::SortOrder sortOrder ){

	switch( sortOrder ){

		case Qt::AscendingOrder:
			return "Qt::AscendingOrder";

		case Qt::DescendingOrder:
			return "Qt::DescendingOrder";

		default:
			return "???";
	}

}/* SortOrderToString() */

void FormsModel::setUpColumns(){

	//const char* sWho = "FormsModel::setUpColumns";

	m_columns.resize( COL_MAX + 1 );

	m_columns[FormsModel::COL_DATE_FILED] = "Date Filed";
	m_columns[FormsModel::COL_ACCESSION_NUMBER] = "Accession Number";
	m_columns[FormsModel::COL_FILER_CIK] = "Filer\nCIK";
	m_columns[FormsModel::COL_FILER_NAME] = "Filer\nName";
	m_columns[FormsModel::COL_FILER_STREET_ADDR] = "Filer\nStreet";
	m_columns[FormsModel::COL_FILER_CITY] = "Filer\nCity";
	m_columns[FormsModel::COL_FILER_CITY] = "Filer\nState";
	m_columns[FormsModel::COL_FILER_CITY] = "Filer\nZip";
	m_columns[FormsModel::COL_FILER_CITY] = "SEC Link";
}

void FormsModel::loadForms()
{
}

// Override for QAbstractTableModel
int FormsModel::rowCount(const QModelIndex & /* parent */) const
{
	//const char* sWho = "FormsModel::rowCount";

	//VF::Logger::log(VF::Logger::INFO) << sWho << "(): returning m_captures->size() = " << m_captures->size() << "..." << endl;
    return m_entries.size();

}/* rowCount() */

// Override for QAbstractTableModel
int FormsModel::columnCount(const QModelIndex & /* parent */) const
{
	//const char* sWho = "FormsModel::columnCount";

	//VF::Logger::log(VF::Logger::INFO) << sWho << "(): returning m_columns.size() = " << m_columns.size() << "..." << endl;

    return m_columns.size(); 

}/* columnCount() */

// Override for QAbstractTableModel
QVariant FormsModel::headerData(int section,
                                   Qt::Orientation orientation,
                                   int role) const
{
	//const char* sWho = "FormsModel::headerData";

	//VF::Logger::log(VF::Logger::INFO) << sWho << "(): section=" << section << "..." << endl;

    if (role == Qt::DisplayRole){

		if( orientation == Qt::Horizontal ){
			if( section >= (int)m_columns.size() ){
				//VF::Logger::log(VF::Logger::INFO) << sWho << "(): Asking for a horizontal column that is >= m_columns.size(), returning an empty QVariant()..." << endl;

   		 		return QVariant();
			}

			//VF::Logger::log(VF::Logger::INFO) << sWho << "(): Returning horizontal column QString( m_columns[section=" << section << "].c_str() = '" << m_columns[section] << "' )..." << endl;

			return QString(m_columns[section].c_str());
		}
		else if( orientation == Qt::Vertical ){

			//VF::Logger::log(VF::Logger::INFO) << sWho << "(): Returning vertical column QVariant( " << (section + 1) << " )..." << endl; 
			return QVariant( section + 1 );
		}

	}

	//VF::Logger::log(VF::Logger::INFO) << sWho << "(): Fell off end, so returning empty QVariant()..." << endl;
	return QVariant();

}/* headerData() */

// Override for QAbstractTableModel
QVariant FormsModel::data(const QModelIndex &index, int role) const
{
	const char* sWho = "FormsModel::data";

	Q_UNUSED(sWho); // Prevent's "unused" compiler warning...I believe it expands to (void)sWho;

	//VF::Logger::log(VF::Logger::INFO) << sWho << "()..." << endl;

    if (!index.isValid()){

		//VF::Logger::log(VF::Logger::INFO) << sWho << "(): index.isValid() is not true, returning empty QVariant()..." << endl;
        return QVariant();

	}

	//VF::Logger::log(VF::Logger::INFO) << sWho << "(): index.row() = " << index.row() << ", index.column() = " << index.column() << "..." << endl;

	if( role == Qt::TextAlignmentRole ){

		//VF::Logger::log(VF::Logger::INFO) << sWho << "(): role == Qt::TextAlignmentRole, returning int(Qt::AlignLeft | Qt::AlignVCenter)..." << endl;
		return int(Qt::AlignLeft | Qt::AlignVCenter);

		//if( index.column() == FormsModel::COL_CLIENT_DRAFT_VERSION ){
		//	return int(Qt::AlignRight | Qt::AlignVCenter);
		//}
		//else {
		//	return int(Qt::AlignLeft | Qt::AlignVCenter);
		//}
	}
	else if( role == Qt::BackgroundRole || role == Qt::DisplayRole ){

		if( index.row() >= (int)m_entries.size() ){
			//VF::Logger::log(VF::Logger::INFO) << sWho << "(): Asking for a row - " << index.row() << " - that is >= m_entries.size(), returning an empty QVariant()..." << endl;

			return QVariant();
		}

		//CapturePtr capturePtr = m_captures.at( m_icaps[index.row()] ); 
		//VF::Logger::log(VF::Logger::INFO) << sWho << "(): capturePtr = " << *(capturePtr) << ", Comrade Captain..." << endl;
		shared_ptr<FormsModel::Entry> p_entry = m_entries.at(index.row());

		if( role == Qt::BackgroundRole ){
		
			//if( index.column() == FormsModel::COL_CATEGORY ){
			//	QBrush backgroundBrush( FormsModel::CategoryTypeToQColor( capturePtr->getCategory() ) );
			//	return backgroundBrush;
			//}

			// Not returning any background brush for now...just empty QVariant()...
			return QVariant();
		
		}
		else if (role == Qt::DisplayRole) {

			//VF::Logger::log(VF::Logger::INFO) << sWho << "(): role == Qt::DisplayRole..." << endl;

			switch( index.column() ){

				case FormsModel::COL_DATE_FILED:
					//VF::Logger::log(VF::Logger::INFO) << sWho << "(): Returning QString( p_entry->date_filed.c_str() = '" << p_entry->date_filed.c_str() << "'..." << endl;
					return QString(p_entry->date_filed.c_str());
					break;

				case FormsModel::COL_ACCESSION_NUMBER:
					//VF::Logger::log(VF::Logger::INFO) << sWho << "(): Returning QString( p_entry->accesion_number.c_str() = '" << p_entry->accesion_number.c_str() << "'..." << endl;
					return QString(p_entry->accession_number.c_str());
					break;

				case FormsModel::COL_FILER_CIK:
					//VF::Logger::log(VF::Logger::INFO) << sWho << "(): Returning QString( p_entry->filer_cik.c_str() = '" << p_entry->filer_cik.c_str() << "'..." << endl;
					return QString(p_entry->filer_cik.c_str());
					break;

				case FormsModel::COL_FILER_NAME:
					//VF::Logger::log(VF::Logger::INFO) << sWho << "(): Returning QString( p_entry->filer_name.c_str() = '" << p_entry->filer_name.c_str() << "'..." << endl;
					return QString(p_entry->filer_name.c_str());
					break;

				case FormsModel::COL_FILER_STREET_ADDR:
					//VF::Logger::log(VF::Logger::INFO) << sWho << "(): Returning QString( p_entry->filer_street_addr.c_str() = '" << p_entry->filer_street_addr.c_str() << "'..." << endl;
					return QString(p_entry->filer_street_addr.c_str());
					break;

				case FormsModel::COL_FILER_CITY:
					//VF::Logger::log(VF::Logger::INFO) << sWho << "(): Returning QString( p_entry->filer_city.c_str() = '" << p_entry->filer_city.c_str() << "'..." << endl;
					return QString(p_entry->filer_city.c_str());
					break;

				case FormsModel::COL_FILER_STATE:
					//VF::Logger::log(VF::Logger::INFO) << sWho << "(): Returning QString( p_entry->filer_state.c_str() = '" << p_entry->filer_state.c_str() << "'..." << endl;
					return QString(p_entry->filer_state.c_str());
					break;

				case FormsModel::COL_FILER_ZIP:
					//VF::Logger::log(VF::Logger::INFO) << sWho << "(): Returning QString( p_entry->filer_zip.c_str() = '" << p_entry->filer_zip.c_str() << "'..." << endl;
					return QString(p_entry->filer_zip.c_str());
					break;

				case FormsModel::COL_SEC_LINK:
					//VF::Logger::log(VF::Logger::INFO) << sWho << "(): Returning QString( p_entry->sec_link.c_str() = '" << p_entry->sec_link.c_str() << "'..." << endl;
					return QString(p_entry->sec_link.c_str());
					break;

				default:
					// We've come up empty, so return an empty QVariant()...
	    			return QVariant();
					break;
	
			}/* switch( index.column() ) */

    	}/* else if (role == Qt::DisplayRole) */

	}/*	else if( role == Qt::BackgroundRole || role == Qt::DisplayRole ){ */

	//VF::Logger::log(VF::Logger::INFO) << sWho << "(): Fell off end of conditionals and switches, empty QVariant()..." << endl;
    return QVariant();

}/* data() */


//void FormsModel::sort ( int column, Qt::SortOrder order ){
//	
//	const char* sWho = "FormsModel::sort";
//
//	Q_UNUSED(sWho);
//
//	VF::Logger::log(VF::Logger::INFO) << sWho << "(): column = " << column << ", order = " << order << " = " << SortOrderToString( order ) << "..." << endl;
//
//	m_comparator.m_sortColumn = column;
//	m_comparator.m_sortOrder = order;
//
//	m_lastSortColumn = column;
//	m_lastSortOrder = order;
//
//	VF::Logger::log(VF::Logger::INFO) << sWho << "(): BEFORE SORT: m_icaps.size()=" << m_icaps.size() << "..." << endl;
//
//	VF::Logger::log(VF::Logger::INFO) << sWho << "():\n";
//	for( size_t i = 0; i < m_icaps.size(); i++ ){
//		VF::Logger::log(VF::Logger::INFO) << " m_icaps[" << i << "] = " << *(m_captures.at( m_icaps[i] )) << "\n";
//	}
//	VF::Logger::log(VF::Logger::INFO) << endl;
//
//	std::sort (m_icaps.begin(), m_icaps.end(), m_comparator);     
//
//	VF::Logger::log(VF::Logger::INFO) << sWho << "(): AFTER SORT: m_icaps.size()=" << m_icaps.size() << "..." << endl;
//
//	VF::Logger::log(VF::Logger::INFO) << sWho << "():\n";
//	for( size_t i = 0; i < m_icaps.size(); i++ ){
//		VF::Logger::log(VF::Logger::INFO) << " m_icaps[" << i << "] = " << *(m_captures.at( m_icaps[i] )) << "\n";
//	}
//	VF::Logger::log(VF::Logger::INFO) << endl;
//
//	emit dataChanged( createIndex(0, 0), createIndex(m_captures.size()-1, m_columns.size()-1) ); 
//}
//
//bool FormsModel::Comparator::operator()(int i, int j){
//
//	const char* sWho = "FormsModel::Comparator::operator()";
//
//	CapturePtr capture_ptr_i = m_parent->m_captures.at( i );	
//	CapturePtr capture_ptr_j = m_parent->m_captures.at( j );	
//
//	int temp_int_i;
//	int temp_int_j;
//
//	bool bReturn;
//
//	switch( m_sortColumn ){
//
//			case FormsModel::COL_VERSION:
//				//VF::Logger::log(VF::Logger::INFO) << sWho << "(): Returning QString( capturePtr->getVersion().c_str() = '" << capturePtr->getVersion().c_str() << "' )..." << endl;
//				bReturn = ( capture_ptr_i->getVersion().compare(capture_ptr_j->getVersion()) < 0 );
//				break;
//
//			case FormsModel::COL_TIMESTAMP:
//				//VF::Logger::log(VF::Logger::INFO) << sWho << "(): Returning QString( capturePtr->getTimeStampUTC().c_str() = '" << capturePtr->getTimeStampNYC().c_str() << "' )..." << endl;
//				bReturn = ( capture_ptr_i->getEpochSeconds() < capture_ptr_j->getEpochSeconds() );
//				break;
//
//			case FormsModel::COL_CATEGORY:
//				//VF::Logger::log(VF::Logger::INFO) << sWho << "(): Returning QString( capturePtr->getComment().c_str() = '" << capturePtr->getComment().c_str() << "' )..." << endl;
//				//bReturn = ( capture_ptr_i->getCategory() < capture_ptr_j->getCategory() );
//				bReturn = ( Capture::categoryToPrettyString( capture_ptr_i->getCategory() ).compare( Capture::categoryToPrettyString( capture_ptr_j->getCategory() ) ) < 0 );
//				break;
//	
//			case FormsModel::COL_CLIENT_DRAFT_VERSION:
//				//VF::Logger::log(VF::Logger::INFO) << sWho << "(): Returning QString( capturePtr->getComment().c_str() = '" << capturePtr->getComment().c_str() << "' )..." << endl;
//				if( sscanf_s(capture_ptr_i->getClientDraftVersion().c_str(), "%d", &temp_int_i)	!= 1 ){
//					temp_int_i = -1;
//				}
//				if( sscanf_s(capture_ptr_j->getClientDraftVersion().c_str(), "%d", &temp_int_j)	!= 1 ){
//					temp_int_j = -1;
//				}
//				bReturn = ( temp_int_i < temp_int_j );
//				break;
//
//			case FormsModel::COL_USERNAME:
//				//VF::Logger::log(VF::Logger::INFO) << sWho << "(): Returning QString( capturePtr->getUserName().c_str() = '" << capturePtr->getUserName().c_str() << "' )..." << endl;
//				bReturn = ( capture_ptr_i->getUserName().compare(capture_ptr_j->getUserName()) < 0 );
//
//				VF::Logger::log(VF::Logger::INFO) << sWho << "(): case FormsModel::COL_USERNAME:\n"
//					<< " capture_ptr_i->getUserName() = '" << capture_ptr_i->getUserName() << "'\n"
//					<< " capture_ptr_j->getUserName() = '" << capture_ptr_j->getUserName() << "'\n"
//					<< "( capture_ptr_i->getUserName().compare(capture_ptr_j->getUserName()) < 0 ) is " 
//					<< VF::Utils::boolToString( bReturn ) << "..." << endl;
//
//				break;
//
//			default:
//				bReturn = true;
//				break;
//
//	}/* switch( m_column ) */
//
//	if( m_sortOrder == Qt::DescendingOrder ){
//		VF::Logger::log(VF::Logger::INFO) << sWho << "( i = " << i << ", j = " << j << " ) m_sortColumn = " << m_sortColumn << " , m_sortOrder = " << m_sortOrder << " : DescendingOrder, so flipping bReturn = " << VF::Utils::boolToString(bReturn) << endl;
//		bReturn = ! bReturn;
//	}
//
//	VF::Logger::log(VF::Logger::INFO) << sWho << "( i = " << i << " = " << *capture_ptr_i << ", j = " << j << " = " << *capture_ptr_j << " ) m_sortColumn = " << m_sortColumn << " , m_sortOrder = " << m_sortOrder << " = " << SortOrderToString( m_sortOrder ) << " : Returning " << VF::Utils::boolToString(bReturn) << "..." << endl;
//	
//	return bReturn;
//
//}/* bool FormsModel::Comparator::operator()(int i, int j) */


ostream& operator<<(ostream& s, const FormsModel::Entry& entry){
	entry.print(s);
	return s;
}
